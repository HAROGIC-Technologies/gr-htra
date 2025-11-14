import sys

# Temporary workaround for SWIG+Python dynamic loading issue
_RTLD_GLOBAL = 0
try:
    from dl import RTLD_GLOBAL as _RTLD_GLOBAL
except ImportError:
    try:
        from DLFCN import RTLD_GLOBAL as _RTLD_GLOBAL
    except ImportError:
        pass

if _RTLD_GLOBAL != 0:
    _dlopenflags = sys.getdlopenflags()
    sys.setdlopenflags(_dlopenflags | _RTLD_GLOBAL)

# import swig generated symbols into the htra namespace
from htra_swig import *

if _RTLD_GLOBAL != 0:
    sys.setdlopenflags(_dlopenflags)



