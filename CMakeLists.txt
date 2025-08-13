########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 3.8)
project(gr-htra_device CXX C)
enable_testing()

# Select the release build type by default to get optimization flags
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: defaulting to release.")
endif()
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

cmake_policy(SET CMP0011 NEW)

########################################################################
# Compiler specific setup
########################################################################

# Check for supported C++ standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C++ standard set to ${CMAKE_CXX_STANDARD}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    AND NOT WIN32)
    add_definitions(-fvisibility=hidden)
endif()

# Set C standard
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
endif()

########################################################################
# Install directories
########################################################################
find_package(Gnuradio "3.9" REQUIRED)
include(GrVersion)
include(GrPlatform) # define LIB_SUFFIX

if(NOT CMAKE_MODULES_DIR)
  set(CMAKE_MODULES_DIR lib${LIB_SUFFIX}/cmake)
endif()

# Install calibration txt files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/CalFile/
        DESTINATION /usr/bin/CalFile
        FILES_MATCHING PATTERN "*.txt")

install(CODE "file(GLOB _files \"\${CMAKE_INSTALL_PREFIX}/bin/CalFile/*.txt\")")
install(CODE "foreach(_file \${_files}) 
    file(SET_PERMISSION \${_file} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
                                       GROUP_READ GROUP_WRITE GROUP_EXECUTE 
                                       WORLD_READ WORLD_WRITE WORLD_EXECUTE) 
endforeach()")

########################################################################
# Add subdirectories (comment out missing directories)
########################################################################
add_subdirectory(include/htra_device)
add_subdirectory(lib)

# GRC and Python bindings
if(ENABLE_PYTHON)
  message(STATUS "PYTHON and GRC components are enabled")
  add_subdirectory(grc)     # Uncomment if grc directory has CMakeLists.txt
  add_subdirectory(bindings)
else()
  message(STATUS "PYTHON and GRC components are disabled")
endif()
