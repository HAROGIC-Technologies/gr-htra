#include "htra_source_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>
#include <cstring>
#include <boost/make_shared.hpp>

namespace gr {
namespace htra {

// helper: trim whitespace
static inline std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

// helper: strip surrounding single or double quotes if present
static inline std::string strip_surrounding_quotes(const std::string &s) {
    if (s.size() >= 2) {
        if ((s.front() == '\'' && s.back() == '\'') ||
            (s.front() == '\"' && s.back() == '\"')) {
            return s.substr(1, s.size() - 2);
        }
    }
    return s;
}

htra_source::sptr htra_source::make(
    const std::string& device_type,int device_num,const std::string& device_ip,float center_freq,float sample_rate, int decim_factor, float ref_level,const std::string& data_format)
{
    return htra_source::sptr(new htra_source_impl(
        device_type, device_num, device_ip, center_freq, sample_rate, decim_factor, ref_level, data_format));
}

htra_source_impl::htra_source_impl(const std::string& device_type,int device_num,const std::string& device_ip,float center_freq,float sample_rate, int decim_factor, float ref_level,const std::string& data_format)
    : gr::sync_block("htra_source",
          gr::io_signature::make(0, 0, 0),
          gr::io_signature::make(1, 1, sizeof(gr_complex))),
      d_center_freq(center_freq),
      d_sample_rate(sample_rate),
      d_decim_factor(decim_factor),
      d_ref_level(ref_level),
      d_rx_thread_running(false),
      d_valid_data_count(0),
      d_read_index(0),
      d_write_index(0),
      d_resyncing(false),
      d_device_num(device_num)
{
    d_boot_profile.DevicePowerSupply = USBPortAndPowerPort;
    //Dynamically configure according to the device_type parameter passed from GRC.
    if (device_type == "USB") {
	d_boot_profile.PhysicalInterface = USB;
	int Status = Device_Open(&d_device, d_device_num, &d_boot_profile, &d_boot_info);
	std::cerr << "USB device opened with status: " << Status << std::endl;
    } else if (device_type == "ETH") {
	d_boot_profile.PhysicalInterface = ETH;
	d_boot_profile.ETH_IPVersion = IPv4;
	d_boot_profile.ETH_RemotePort = 5000;
	d_boot_profile.ETH_ReadTimeOut = 5000;

	std::string ip_clean = trim(device_ip);
	ip_clean = strip_surrounding_quotes(ip_clean);
	int ip1, ip2, ip3, ip4;
	if (sscanf(ip_clean.c_str(), "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4) != 4) {
    	throw std::runtime_error("Invalid IP address format");
	}
	d_boot_profile.ETH_IPAddress[0] = ip1;
 	d_boot_profile.ETH_IPAddress[1] = ip2;
	d_boot_profile.ETH_IPAddress[2] = ip3;
	d_boot_profile.ETH_IPAddress[3] = ip4;

	int Status = Device_Open(&d_device, 0, &d_boot_profile, &d_boot_info);
	std::cerr << "ETH device opened with status: " << Status << std::endl;
    } else {
	throw std::runtime_error("Unknown device type");
    }
    int Status = IQS_ProfileDeInit(&d_device, &IQS_ProfileIn);
    std::cerr << "IQS_ProfileDeInit status: " << Status << std::endl;

    IQS_ProfileIn.CenterFreq_Hz = d_center_freq;
    IQS_ProfileIn.RefLevel_dBm = d_ref_level;
    IQS_ProfileIn.DecimateFactor = static_cast<int>(d_decim_factor);
    
    if(data_format=="Complex16bit"){
        IQS_ProfileIn.DataFormat = Complex16bit;
    } else if(data_format=="Complex8bit"){
        IQS_ProfileIn.DataFormat = Complex8bit;
    } else if(data_format=="Complex32bit"){
        IQS_ProfileIn.DataFormat = Complex32bit;
    } else {
        throw std::runtime_error("Unsupported data format");
    }

    IQS_ProfileIn.TriggerSource = Bus;
    IQS_ProfileIn.TriggerMode = Adaptive;
    IQS_ProfileIn.NativeIQSampleRate_SPS = d_sample_rate;
    IQS_ProfileIn.BusTimeout_ms = 5000;

    Status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
    std::cerr << "Native Sampling Rate: " << d_profile_out.NativeIQSampleRate_SPS << std::endl;
    std::cerr << "DecimateFactor: " << d_profile_out.DecimateFactor << std::endl;
    std::cerr << "IQSampleRate: " << d_stream_info.IQSampleRate << std::endl;

    d_ring_buffer.resize(32484*128);

    Status = IQS_BusTriggerStart(&d_device);
    std::cerr << "IQS_BusTriggerStart status: " << Status << std::endl;

    activateStream();
}

htra_source_impl::~htra_source_impl()
{
    if (d_rx_thread_running) {
        d_rx_thread_running = false;
        d_buffer_cv.notify_all();
        d_rx_thread.join();
    }
    Device_Close(&d_device);
}

void htra_source_impl::set_sample_rate(float rate)
{
    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = true;
        d_valid_data_count = 0;
        d_read_index = 0;
        d_write_index = 0;
        d_buffer_cv.notify_all();
    }

    d_sample_rate = rate;
    IQS_ProfileIn.NativeIQSampleRate_SPS = d_sample_rate;

    int status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
    std::cerr << "[Reconfig set_sample_rate] status: " << status
              << " NativeSamplingRate: " << d_profile_out.NativeIQSampleRate_SPS << std::endl;

    IQS_BusTriggerStart(&d_device);

    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = false;
    }
    d_buffer_cv.notify_all();
}

void htra_source_impl::set_center_freq(float freq)
{
    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = true;
        d_valid_data_count = 0;
        d_read_index = 0;
        d_write_index = 0;
        d_buffer_cv.notify_all();
    }

    d_center_freq = freq;
    IQS_ProfileIn.CenterFreq_Hz = d_center_freq;

    int status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
    std::cerr << "[Reconfig set_center_freq] status: " << status
              << " CenterFreq_Hz: " << d_profile_out.CenterFreq_Hz << std::endl;

    IQS_BusTriggerStart(&d_device);

    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = false;
    }
    d_buffer_cv.notify_all();
}

void htra_source_impl::set_ref_level(float level)
{
    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = true;
        d_valid_data_count = 0;
        d_read_index = 0;
        d_write_index = 0;
        d_buffer_cv.notify_all();
    }

    d_ref_level = level;
    IQS_ProfileIn.RefLevel_dBm = d_ref_level;

    int status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
    std::cerr << "[Reconfig set_ref_level] status: " << status
              << " RefLevel_dBm: " << d_profile_out.RefLevel_dBm << std::endl;

    IQS_BusTriggerStart(&d_device);

    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = false;
    }
    d_buffer_cv.notify_all();
}

int htra_source_impl::activateStream()
{
    if (d_rx_thread_running)
        return 0;
    d_rx_thread_running = true;
    d_rx_thread = std::thread(&htra_source_impl::_rx_thread, this);
    return 0;
}

int htra_source_impl::deactivateStream()
{
    if (!d_rx_thread_running)
        return 0;
    d_rx_thread_running = false;
    d_buffer_cv.notify_all();
    if (d_rx_thread.joinable())
        d_rx_thread.join();
    return 0;
}

//Data Acquisition Thread
void htra_source_impl::_rx_thread()
{
    const size_t BLOCKS_PER_FETCH = 8;
    std::vector<std::complex<float>> buffer;
    int Status = 0;
    bool flag=true;
    while (d_rx_thread_running) {
    
    	{
            std::unique_lock<std::mutex> lock(d_buffer_mutex);
            d_buffer_cv.wait(lock, [this]() {
                return (!d_resyncing && d_rx_thread_running) || !d_rx_thread_running;
            });

            if (!d_rx_thread_running)
                break;
        }

        for(int t = 0 ; t < BLOCKS_PER_FETCH && d_rx_thread_running ; t++){
            {
        	std::unique_lock<std::mutex>  inner_lock(d_buffer_mutex);
        	if (d_resyncing) break; 
    	    }
        
            Status = IQS_GetIQStream_PM1(&d_device, &d_iq_stream); // Acquire data from device
            if(flag){
            	std::cerr << "d_iq_stream.IQS_ScaleToV: " << d_iq_stream.IQS_ScaleToV << std::endl;
            	flag=false;
            }
            if(Status==-1){
                std::cerr << "IQS_GetIQStream_PM1 status: " << Status << std::endl;
                std::exit(EXIT_FAILURE);
            }
            if(Status==10060||Status==10054||Status==10062){
                std::cerr << "IQS_GetIQStream_PM1 status: " << Status << std::endl;
                Device_Close(&d_device);
                Status = Device_Open(&d_device, d_device_num, &d_boot_profile, &d_boot_info);
                std::cerr << "Device_Open Status: " << Status << std::endl;
                if(Status!=0){
                    std::exit(EXIT_FAILURE);
                } 
                Status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
                std::cerr << "IQS_Configuration Status: " << Status << std::endl;
                if(Status!=0){
                    std::exit(EXIT_FAILURE);
                }
                continue;
            }
            if (Status==-10) {
            	std::cerr << "IQS_GetIQStream_PM1 status: " << Status << std::endl;
            	// Device returns -10, indicating desync. Reset buffer:
	    	// Both write and read positions start from the beginning.
                {
                    std::lock_guard<std::mutex> lock(d_buffer_mutex);
                    d_resyncing = true;  
                    d_valid_data_count = 0;
                    d_read_index = 0;
                    d_write_index = 0;
                }
                d_buffer_cv.notify_all();

                Status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
                std::cerr << "IQS_Configuration Status: " << Status << std::endl;
                if(Status!=0){
                    Status = Device_Open(&d_device, d_device_num, &d_boot_profile, &d_boot_info);
                    std::cerr << "Device_Open Status: " << Status << std::endl;
                    if(Status!=0){
                        std::exit(EXIT_FAILURE);
                    }
                    Status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
                    std::cerr << "IQS_Configuration Status: " << Status << std::endl;
                    if(Status!=0){
                        std::exit(EXIT_FAILURE);
                    }
                }
                IQS_BusTriggerStart(&d_device);

                {
                    std::lock_guard<std::mutex> lock(d_buffer_mutex);
                    d_resyncing = false;  
                }
                continue;
            }

            if (Status==-12) {
                std::cerr << "Overflow: " << Status << std::endl;
            }

            // Store acquired data into buffer
            size_t packetSamples = d_iq_stream.IQS_StreamInfo.PacketSamples; 
            if (packetSamples == 0) continue;
            
            if (t == 0) {
                buffer.clear();
                buffer.resize(packetSamples * BLOCKS_PER_FETCH);
            }

            size_t base_idx = t * packetSamples;

            switch (d_iq_stream.IQS_Profile.DataFormat) {
                case Complex8bit: {
                    int8_t* iq8 = reinterpret_cast<int8_t*>(d_iq_stream.AlternIQStream);
                    for (size_t i = 0; i < packetSamples; ++i) {
                        buffer[i + base_idx].real(iq8[2*i]*d_iq_stream.IQS_ScaleToV);
                        buffer[i + base_idx].imag(iq8[2*i+1]*d_iq_stream.IQS_ScaleToV);
                    }
                    break;
                }
                case Complex16bit: {
                    int16_t* iq16 = reinterpret_cast<int16_t*>(d_iq_stream.AlternIQStream);
                    for (size_t i = 0; i < packetSamples; ++i) {
                        buffer[i + base_idx].real(iq16[2*i]*d_iq_stream.IQS_ScaleToV);
                        buffer[i + base_idx].imag(iq16[2*i+1]*d_iq_stream.IQS_ScaleToV);
                    }
                    break;
                }
                case Complex32bit: {
                    int32_t* iq32 = reinterpret_cast<int32_t*>(d_iq_stream.AlternIQStream);
                    for (size_t i = 0; i < packetSamples; ++i) {
                        buffer[i + base_idx].real(iq32[2*i]*d_iq_stream.IQS_ScaleToV);
                        buffer[i + base_idx].imag(iq32[2*i+1]*d_iq_stream.IQS_ScaleToV);
                    }
                    break;
                }
                default:
                    std::cerr << "Unsupported IQ format: " << d_iq_stream.IQS_Profile.DataFormat << std::endl;
                    continue;
            }
        }


        size_t actual_filled = buffer.size();
        if (actual_filled == 0) continue;

        {
            std::lock_guard<std::mutex> lock(d_buffer_mutex);
            
	    // When the ring buffer is full, reset read and write indices to restart from the beginning.
            if (d_valid_data_count + actual_filled > d_ring_buffer.size()) {
                std::cerr << "T";
                d_valid_data_count = 0;
                d_read_index = 0;
                d_write_index = 0;
            }
	    
	    // Write acquired IQ data into the ring buffer.
            size_t space_to_end = d_ring_buffer.size() - d_write_index;
            if (buffer.size() <= space_to_end) {
                std::copy(buffer.begin(), buffer.end(), d_ring_buffer.begin() + d_write_index);
            } else {
                std::copy(buffer.begin(), buffer.begin() + space_to_end, d_ring_buffer.begin() + d_write_index);
                std::copy(buffer.begin() + space_to_end, buffer.end(), d_ring_buffer.begin());
            }

            d_write_index = (d_write_index + buffer.size()) % d_ring_buffer.size();
            d_valid_data_count += buffer.size();
        }

	// Notify main thread that data is available
        d_buffer_cv.notify_one(); 
    }
}

int htra_source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
    gr_complex* out = (gr_complex*)output_items[0];

    std::unique_lock<std::mutex> lock(d_buffer_mutex);
    d_buffer_cv.wait(lock, [this, noutput_items]() {
        return (!d_resyncing && d_valid_data_count >= (size_t)noutput_items) || !d_rx_thread_running;
    });

    if (!d_rx_thread_running && d_valid_data_count < (size_t)noutput_items)
        return WORK_DONE;

    size_t total_samples = static_cast<size_t>(noutput_items);
    size_t available_to_end = d_ring_buffer.size() - d_read_index;


    if (total_samples <= available_to_end) {
        std::copy_n(d_ring_buffer.begin() + d_read_index, total_samples, out);
    } else {
        std::copy_n(d_ring_buffer.begin() + d_read_index, available_to_end, out);
        std::copy_n(d_ring_buffer.begin(), total_samples - available_to_end, out + available_to_end);
    }

    d_read_index = (d_read_index + total_samples) % d_ring_buffer.size();
    d_valid_data_count -= total_samples;

    return noutput_items;
}

gr::basic_block_sptr htra_source_impl::to_basic_block()
{
    return shared_from_this();
}

} // namespace htra
} // namespace gr

