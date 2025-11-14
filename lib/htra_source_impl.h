#ifndef INCLUDED_HTRA_SOURCE_IMPL_H
#define INCLUDED_HTRA_SOURCE_IMPL_H

#include <htra/htra_source.h>
#include <thread>
#include <htra_api.h>
#include <mutex>
#include <condition_variable>
#include <vector>

namespace gr {
namespace htra {

class htra_source_impl : public htra_source
{
private:
    void* d_device; 
    float d_center_freq;
    float d_sample_rate;
    int d_decim_factor;
    float d_ref_level;
    int d_device_num;
    BootProfile_TypeDef d_boot_profile;
    BootInfo_TypeDef d_boot_info;
    IQS_Profile_TypeDef d_profile_out;    
    IQS_StreamInfo_TypeDef d_stream_info;  
    IQS_Profile_TypeDef IQS_ProfileIn;
    IQStream_TypeDef d_iq_stream;

    std::thread d_rx_thread;
    std::mutex d_buffer_mutex;
    std::condition_variable d_buffer_cv;
    bool d_rx_thread_running;
    bool d_resyncing;
    size_t d_valid_data_count;
    size_t d_read_index;
    size_t d_write_index;
    std::vector<std::complex<float>> d_ring_buffer;
    void _rx_thread();

public:
    htra_source_impl(const std::string& device_type,int device_num,const std::string& device_ip,float center_freq,float sample_rate, int decim, float ref_level,const std::string& data_format);
    ~htra_source_impl();

    void set_sample_rate(float rate) override;
    void set_center_freq(float freq) override;
    void set_ref_level(float level) override;

    int activateStream() override;
    int deactivateStream() override;
    int work(int noutput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items) override;

    gr::basic_block_sptr to_basic_block() override;

};

} // namespace htra
} // namespace gr

#endif /* INCLUDED_HTRA_SOURCE_IMPL_H */

