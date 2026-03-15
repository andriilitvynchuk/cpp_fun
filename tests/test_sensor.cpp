#include "start/sensor.h"

int main() {
    CameraSensor cam;
    if (cam.name() != "Camera") return 1;
    if (cam.read_frame() != 0) return 1;
    if (cam.read_frame() != 1) return 1;

    DummySensor dummy;
    if (dummy.name() != "Dummy") return 1;
    if (dummy.read_frame() != -1) return 1;

    return 0;
}
