/* -*- c++ -*- */

#define HTRA_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "htra_swig_doc.i"

%{
#include "htra/htra_source.h"
%}


%include "htra/htra_source.h"
GR_SWIG_BLOCK_MAGIC2(htra, htra_source);
