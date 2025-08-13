#include <pybind11/pybind11.h>
#include <htra_device/htra_source.h>

namespace py = pybind11;

void bind_htra_source(py::module& m)
{
    using htra_source = gr::htra_device::htra_source;

    py::class_<htra_source, std::shared_ptr<htra_source>>(m, "htra_source")
        .def(py::init(&htra_source::make),
             py::arg("center_freq"),
             py::arg("sample_rate"),
             py::arg("decim_factor"),
             py::arg("ref_level"))
        .def("set_sample_rate", &gr::htra_device::htra_source::set_sample_rate)
        .def("set_center_freq", &gr::htra_device::htra_source::set_center_freq)
	.def("set_ref_level", &gr::htra_device::htra_source::set_ref_level)
	.def("set_decim_factor", &gr::htra_device::htra_source::set_decim_factor)
        .def("to_basic_block", &gr::htra_device::htra_source::to_basic_block) 
        .def("activateStream", &gr::htra_device::htra_source::activateStream)
        .def("deactivateStream", &gr::htra_device::htra_source::deactivateStream)
        .def("work", &gr::htra_device::htra_source::work);
}

PYBIND11_MODULE(htra_source, m)
{
    bind_htra_source(m);
}


