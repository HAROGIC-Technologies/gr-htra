/* -*- c++ -*- */
/* 
 * Copyright 2025 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_HTRA_HTRA_SOURCE_H
#define INCLUDED_HTRA_HTRA_SOURCE_H

#include <htra/api.h>
#include <gnuradio/sync_block.h>


namespace gr {
  namespace htra {

    /*!
     * \brief <+description of block+>
     * \ingroup htra
     *
     */
    class HTRA_API htra_source : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<htra_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of htra::htra_source.
       *
       * To avoid accidental use of raw pointers, htra::htra_source's
       * constructor is in a private implementation
       * class. htra::htra_source::make is the public interface for
       * creating new instances.
       */
          static sptr make(const std::string& device_type,int device_num,const std::string& device_ip,float center_freq,float sample_rate,  int decim_factor , float ref_level,const std::string& data_format);  


    virtual gr::basic_block_sptr to_basic_block() = 0;

virtual void set_sample_rate(float rate) = 0;
virtual void set_center_freq(float freq) = 0;
virtual void set_ref_level(float level) = 0;

    virtual int activateStream() = 0;


    virtual int deactivateStream() = 0;


    virtual int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items) = 0;
                     

};

  } // namespace htra
} // namespace gr

#endif /* INCLUDED_HTRA_HTRA_SOURCE_H */

