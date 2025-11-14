INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_HTRA htra)

FIND_PATH(
    HTRA_INCLUDE_DIRS
    NAMES htra/api.h
    HINTS $ENV{HTRA_DIR}/include
        ${PC_HTRA_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    HTRA_LIBRARIES
    NAMES gnuradio-htra
    HINTS $ENV{HTRA_DIR}/lib
        ${PC_HTRA_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HTRA DEFAULT_MSG HTRA_LIBRARIES HTRA_INCLUDE_DIRS)
MARK_AS_ADVANCED(HTRA_LIBRARIES HTRA_INCLUDE_DIRS)

