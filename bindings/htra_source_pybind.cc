#include <pybind11/pybind11.h>
#include <htra_device/htra_source.h>

namespace py = pybind11;

void bind_htra_source(py::module& m)
{
    using htra_source = gr::htra_device::htra_source;
    using DecimationFactor = gr::htra_device::DecimationFactor;
    
    py::enum_<DecimationFactor>(m, "DecimationFactor")
        .value("DECIM_1", DecimationFactor::DECIM_1)
        .value("DECIM_2", DecimationFactor::DECIM_2)
        .value("DECIM_4", DecimationFactor::DECIM_4)
        .value("DECIM_8", DecimationFactor::DECIM_8)
        .value("DECIM_16", DecimationFactor::DECIM_16)
        .value("DECIM_32", DecimationFactor::DECIM_32)
        .value("DECIM_64", DecimationFactor::DECIM_64)
        .value("DECIM_128", DecimationFactor::DECIM_128)
        .value("DECIM_256", DecimationFactor::DECIM_256)
        .value("DECIM_512", DecimationFactor::DECIM_512)
        .value("DECIM_1024", DecimationFactor::DECIM_1024)
        .value("DECIM_2048", DecimationFactor::DECIM_2048)
        .value("DECIM_4096", DecimationFactor::DECIM_4096)
        .export_values();
    
    py::class_<htra_source, std::shared_ptr<htra_source>>(m, "htra_source")
        .def(py::init(&htra_source::make),
             py::arg("center_freq"),
             py::arg("sample_rate"),
             py::arg("decim_factor"),
             py::arg("ref_level"))
        .def("set_sample_rate", &htra_source::set_sample_rate)
        .def("set_center_freq", &htra_source::set_center_freq)
        .def("set_ref_level", &htra_source::set_ref_level)
        .def("set_decim_factor", &htra_source::set_decim_factor)
        .def("to_basic_block", &htra_source::to_basic_block) 
        .def("activateStream", &htra_source::activateStream)
        .def("deactivateStream", &htra_source::deactivateStream)
        .def("work", &htra_source::work);

    
}

PYBIND11_MODULE(htra_source, m)
{
    bind_htra_source(m);
}


