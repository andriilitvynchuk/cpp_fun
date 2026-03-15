#include "start/sensor.h"

#include <iostream>

// CameraSensor
int CameraSensor::next_frame_id_ = 0;

std::string CameraSensor::name() const { return "Camera"; }

int CameraSensor::read_frame() { return next_frame_id_++; }

// DummySensor
std::string DummySensor::name() const { return "Dummy"; }

int DummySensor::read_frame() { return -1; }

// Free functions

void use_sensor(Sensor& s) {
    std::cout << "sensor: " << s.name() << ", frame " << s.read_frame() << std::endl;
}

void abstract_class_examples() {
    CameraSensor cam;
    DummySensor dummy;
    use_sensor(cam);
    use_sensor(cam);
    use_sensor(dummy);

    Sensor* ptr = &cam;
    std::cout << ptr->name() << std::endl;

    std::unique_ptr<Sensor> sensor = std::make_unique<CameraSensor>();
    use_sensor(*sensor);
}
