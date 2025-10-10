########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 3.8)
project(gr-htra_device CXX C)
enable_testing()

# Default build type
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: defaulting to Release.")
endif()
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

cmake_policy(SET CMP0011 NEW)

########################################################################
# Compiler setup
########################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "C++ standard set to ${CMAKE_CXX_STANDARD}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU") AND NOT WIN32)
    add_definitions(-fvisibility=hidden)
endif()

########################################################################
# Dependencies
########################################################################
find_package(Gnuradio "3.9" REQUIRED)
include(GrVersion)
include(GrPlatform) # define LIB_SUFFIX

if(NOT CMAKE_MODULES_DIR)
  set(CMAKE_MODULES_DIR lib${LIB_SUFFIX}/cmake)
endif()

########################################################################
# Pre-install htraapi libraries and headers via install_lib.sh
########################################################################
set(INSTALL_SH "${CMAKE_SOURCE_DIR}/install_lib.sh")


file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/set_install_sh_permissions.sh"
     CONTENT "#!/bin/bash\nchmod +x ${INSTALL_SH}")

add_custom_target(make_install_sh_executable ALL
    COMMAND /bin/bash ${CMAKE_BINARY_DIR}/set_install_sh_permissions.sh
    COMMENT "Making install_lib.sh executable"
)

add_custom_target(install_htraapi ALL
    COMMAND /bin/bash ${INSTALL_SH}
    COMMENT "Installing htraapi libs/headers during build"
)


add_dependencies(install_htraapi make_install_sh_executable)

########################################################################
# Add module subdirectories
########################################################################
add_subdirectory(include/htra_device)
add_subdirectory(lib)

# Python / GRC bindings
if(ENABLE_PYTHON)
  message(STATUS "PYTHON and GRC components are enabled")
  add_subdirectory(grc)
  add_subdirectory(bindings)
else()
  message(STATUS "PYTHON and GRC components are disabled")
endif()

