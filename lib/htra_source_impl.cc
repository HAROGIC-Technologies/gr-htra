
#include "htra_source_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>
#include <cstring>
#include <atomic>

namespace gr {
namespace htra_device {

htra_source::sptr htra_source::make(
    float center_freq,float sample_rate, float decim_factor, float ref_level)
{
    return gnuradio::make_block_sptr<htra_source_impl>(
        center_freq, sample_rate, decim_factor, ref_level);
}

htra_source_impl::htra_source_impl(float center_freq,float sample_rate, float decim_factor, float ref_level)
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
      d_resyncing(false)
{
    //Initialize Device
    BootProfile_TypeDef BootProfile;
    BootInfo_TypeDef BootInfo;
    BootProfile.DevicePowerSupply = USBPortAndPowerPort;
    BootProfile.PhysicalInterface = USB;

    int Status = Device_Open(&d_device, 0, &BootProfile, &BootInfo);
    std::cerr << "Device opened with status: " << Status << std::endl;
    
    //Initialize IQS Profile
    Status = IQS_ProfileDeInit(&d_device, &IQS_ProfileIn);
    std::cerr << "IQS_ProfileDeInit status: " << Status << std::endl;
    
    IQS_ProfileIn.CenterFreq_Hz = d_center_freq;
    IQS_ProfileIn.RefLevel_dBm = d_ref_level;
    IQS_ProfileIn.DecimateFactor = d_decim_factor;
    IQS_ProfileIn.DataFormat = Complex16bit;
    IQS_ProfileIn.TriggerSource = Bus;
    IQS_ProfileIn.TriggerMode = Adaptive;
    IQS_ProfileIn.NativeIQSampleRate_SPS = d_sample_rate;
    IQS_ProfileIn.BusTimeout_ms=5000;

    Status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
    std::cerr << "Native Sampling Rate: " << d_profile_out.NativeIQSampleRate_SPS << std::endl;
    std::cerr << "DecimateFactor: " << d_profile_out.DecimateFactor << std::endl;
    std::cerr << "IQSampleRate: " << d_stream_info.IQSampleRate << std::endl;
    

    // Allocate Ring Buffer
    d_ring_buffer.resize(16242*512); 
    
    //Trigger Start of Acquisition
    Status = IQS_BusTriggerStart(&d_device);
    std::cerr << "IQS_BusTriggerStart status: " << Status << std::endl;

    // Start data acquisition stream
    activateStream();
}

//Reconfig
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
    std::cerr << "[Native Sampling Rate] Reconfig status: " << status << std::endl;
    std::cerr << "Native Sampling Rate: " << d_profile_out.NativeIQSampleRate_SPS << std::endl;
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
    std::cerr << "[CenterFreq] Reconfig status: " << status << std::endl;
    std::cerr << "d_profile_out.CenterFreq_Hz: " << d_profile_out.CenterFreq_Hz << std::endl;
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
    std::cerr << "[RefLevel] Reconfig status: " << status << std::endl;
    std::cerr << "d_profile_out.RefLevel_dBm: " << d_profile_out.RefLevel_dBm << std::endl;
    status=IQS_BusTriggerStart(&d_device);


    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = false;
    }
    d_buffer_cv.notify_all();
}

void htra_source_impl::set_decim_factor(float decim)
{
    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = true;
        d_valid_data_count = 0;
        d_read_index = 0;
        d_write_index = 0;
        d_buffer_cv.notify_all();
    }

    d_decim_factor = decim;
    IQS_ProfileIn.DecimateFactor = static_cast<int>(d_decim_factor);

    int status = IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
    std::cerr << "[DecimateFactor] Reconfig status: " << status << std::endl;
    std::cerr << "d_profile_out.DecimateFactor: " << d_profile_out.DecimateFactor << std::endl;
    IQS_BusTriggerStart(&d_device);

    {
        std::lock_guard<std::mutex> lock(d_buffer_mutex);
        d_resyncing = false;
    }
    d_buffer_cv.notify_all();
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

int htra_source_impl::activateStream()
{
    if (d_rx_thread_running) return 0;  // Do not start if already running
    d_rx_thread_running = true;
    d_rx_thread = std::thread(&htra_source_impl::_rx_thread, this);
    return 0;
}

// Stop data acquisition stream
int htra_source_impl::deactivateStream()
{
    if (!d_rx_thread_running) return 0; 
    d_rx_thread_running = false;
    d_buffer_cv.notify_all(); 
    if (d_rx_thread.joinable()) {
        d_rx_thread.join(); 
    }
    return 0;
}


//Data Acquisition Thread
void htra_source_impl::_rx_thread()
{
    std::vector<std::complex<float>> buffer(16242*8);
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

        for(int t = 0 ; t < 8 ; t++){
            {
        	std::unique_lock<std::mutex> lock(d_buffer_mutex);
        	if (d_resyncing) break; 
    	    }
        
            Status = IQS_GetIQStream_PM1(&d_device, &d_iq_stream); // Acquire data from device
            if(flag){
            	std::cerr << "d_iq_stream.IQS_ScaleToV: " << d_iq_stream.IQS_ScaleToV << std::endl;
            	flag=false;
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

                IQS_Configuration(&d_device, &IQS_ProfileIn, &d_profile_out, &d_stream_info);
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
            int16_t* iq_data = (int16_t*)d_iq_stream.AlternIQStream;
            

            for (size_t i = 0; i < d_iq_stream.IQS_StreamInfo.PacketSamples; i++) {
                buffer[i + t * d_iq_stream.IQS_StreamInfo.PacketSamples].real(iq_data[2 * i] * d_iq_stream.IQS_ScaleToV);
                buffer[i + t * d_iq_stream.IQS_StreamInfo.PacketSamples].imag(iq_data[2 * i + 1] * d_iq_stream.IQS_ScaleToV);
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(d_buffer_mutex);
            
	    // When the ring buffer is full, reset read and write indices to restart from the beginning.
            if (d_valid_data_count + buffer.size() > d_ring_buffer.size()) {
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
    gr_complex* out = (gr_complex*) output_items[0];
    // Wait for available data in the buffer before forwarding it to downstream processing modules.
    std::unique_lock<std::mutex> lock(d_buffer_mutex);
    d_buffer_cv.wait(lock, [this, noutput_items]() {return (!d_resyncing && d_valid_data_count >= noutput_items) || !d_rx_thread_running;});

   
    if (!d_rx_thread_running && d_valid_data_count < noutput_items) {
        return WORK_DONE; 
    }
    
    // Retrieve data from the ring buffer and pass it to the downstream block.
    size_t space_to_end = d_ring_buffer.size() - d_read_index;
    if (noutput_items <= space_to_end) {
        std::copy(d_ring_buffer.begin() + d_read_index,d_ring_buffer.begin() + d_read_index + noutput_items,out);
    } else {
        std::copy(d_ring_buffer.begin() + d_read_index,d_ring_buffer.end(),out);
        std::copy(d_ring_buffer.begin(), d_ring_buffer.begin() + (noutput_items - space_to_end),out + space_to_end);
    }

    d_read_index = (d_read_index + noutput_items) % d_ring_buffer.size();
    d_valid_data_count -= noutput_items;

    return noutput_items;
}

// Convert to basic block
gr::basic_block_sptr htra_source_impl::to_basic_block()
{
    return shared_from_this();
}

} // namespace htra_device
} // namespace gr

