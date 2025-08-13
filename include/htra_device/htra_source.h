#ifndef INCLUDED_HTRA_DEVICE_SOURCE_H
#define INCLUDED_HTRA_DEVICE_SOURCE_H

#include <gnuradio/sync_block.h>

namespace gr {
namespace htra_device {

class htra_source : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<htra_source> sptr;


    static sptr make(float center_freq,float sample_rate, float decim_factor, float ref_level);  

  
    virtual gr::basic_block_sptr to_basic_block() = 0;
    
    virtual void set_sample_rate(float rate) = 0;
    virtual void set_center_freq(float freq) = 0;
    virtual void set_ref_level(float level) = 0;
    virtual void set_decim_factor(float decim) = 0;


    virtual int activateStream() = 0;


    virtual int deactivateStream() = 0;


    virtual int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items) = 0;
                     

};

} // namespace htra_device
} // namespace gr

#endif /* INCLUDED_HTRA_DEVICE_SOURCE_H */


