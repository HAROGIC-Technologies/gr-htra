#ifndef INCLUDED_HTRA_DEVICE_SOURCE_IMPL_H
#define INCLUDED_HTRA_DEVICE_SOURCE_IMPL_H

#include <htra_device/htra_source.h>
#include <htra_api.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace gr {
namespace htra_device {

class htra_source_impl : public htra_source
{
private:
    void* d_device;                          
    float d_center_freq, d_sample_rate, d_ref_level;
    int d_device_num;
    BootProfile_TypeDef d_boot_profile;
    BootInfo_TypeDef d_boot_info;
    DecimationFactor d_decim_factor;
    IQS_Profile_TypeDef d_profile_out;    
    IQS_StreamInfo_TypeDef d_stream_info;  
    IQS_Profile_TypeDef IQS_ProfileIn;
    IQStream_TypeDef d_iq_stream;       
    size_t d_valid_data_count;   // Amount of valid data in the buffer
    size_t d_read_index;         // Read index in ring buffer
    size_t d_write_index;        // Write index in ring buffer

    // Ring buffer for storing acquired IQ data
    std::vector<gr_complex> d_ring_buffer;

    // Thread synchronization primitives
    std::mutex d_buffer_mutex;                   // Mutex for protecting the buffer
    std::condition_variable d_buffer_cv;         // Condition variable for buffer signaling
    std::atomic<bool> d_resyncing;               // Indicates if resynchronization is in progress
    bool d_rx_thread_running;                    // Flag for acquisition thread status
    std::thread d_rx_thread;                     // Data acquisition thread

    // Data acquisition thread routine
    void _rx_thread();

public:
    htra_source_impl(const std::string& device_type,int device_num,const std::string& device_ip,float center_freq,float sample_rate, DecimationFactor decim, float ref_level,const std::string& data_format);
    ~htra_source_impl();
    
    void set_sample_rate(float rate) override;
    void set_center_freq(float freq) override;
    void set_ref_level(float level) override;
    void set_decim_factor(DecimationFactor decim) override;

    // Start the data acquisition stream
    int activateStream() override;

    // Stop the data acquisition stream
    int deactivateStream() override;
    
    // Read data from buffer and forward to output
    int work(int noutput_items, 
             gr_vector_const_void_star &input_items, 
             gr_vector_void_star &output_items) override;

    // Convert to basic_block (required by GNU Radio)
    gr::basic_block_sptr to_basic_block() override;
};

} // namespace htra_device
} // namespace gr

#endif /* INCLUDED_HTRA_DEVICE_SOURCE_IMPL_H */


