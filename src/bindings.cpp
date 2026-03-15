#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "start/sensor.h"

namespace py = pybind11;

// Trampoline so Python can subclass Sensor and override virtuals.
struct PySensor : Sensor {
    using Sensor::Sensor;
    std::string name() const override { PYBIND11_OVERRIDE_PURE(std::string, Sensor, name, ); }
    int read_frame() override { PYBIND11_OVERRIDE_PURE(int, Sensor, read_frame, ); }
};

PYBIND11_MODULE(_start_lib, m) {
    m.doc() = "start C++ library: Sensor API";

    py::class_<Sensor, PySensor, std::shared_ptr<Sensor>>(m, "Sensor")
        .def("name", &Sensor::name)
        .def("read_frame", &Sensor::read_frame);

    py::class_<CameraSensor, Sensor, std::shared_ptr<CameraSensor>>(m, "CameraSensor")
        .def(py::init<>())
        .def("name", &CameraSensor::name)
        .def("read_frame", &CameraSensor::read_frame);

    py::class_<DummySensor, Sensor, std::shared_ptr<DummySensor>>(m, "DummySensor")
        .def(py::init<>())
        .def("name", &DummySensor::name)
        .def("read_frame", &DummySensor::read_frame);

    m.def("use_sensor", &use_sensor, py::arg("sensor"),
          "Call read_frame and print sensor name and frame id.");
    m.def("abstract_class_examples", &abstract_class_examples,
          "Run C++ sensor examples (CameraSensor, DummySensor, use_sensor).");
}
