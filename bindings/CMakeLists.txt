find_package(pybind11 REQUIRED)

pybind11_add_module(htra_source MODULE
    htra_source_pybind.cc
)

target_link_libraries(htra_source
    PRIVATE gnuradio-htra_device
)

install(
    TARGETS htra_source DESTINATION ${GR_PYTHON_DIR}
)
