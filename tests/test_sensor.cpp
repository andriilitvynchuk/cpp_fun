#include <iostream>

#include "start/sensor.h"

int main() {
    CameraSensor cam;
    if (cam.name() != "Camera") throw std::runtime_error("Camera name is not Camera");
    if (cam.read_frame() != 0) throw std::runtime_error("Camera read frame is not 0");
    if (cam.read_frame() != 1) throw std::runtime_error("Camera read frame is not 1");

    DummySensor dummy;
    if (dummy.name() != "Dummy") throw std::runtime_error("Dummy name is not Dummy");
    if (dummy.read_frame() != -1) throw std::runtime_error("Dummy read frame is not -1");
    std::cout << "All tests passed" << std::endl;
    return 0;
}
