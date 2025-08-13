set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(gnuradio-htra_device
    htra_source_impl.cc
)

target_include_directories(gnuradio-htra_device PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(gnuradio-htra_device
    gnuradio-runtime
    ${GNURADIO_ALL_LIBRARIES}
    htraapi
)

install(TARGETS gnuradio-htra_device DESTINATION lib)

