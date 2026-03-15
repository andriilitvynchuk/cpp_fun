// ============== Step 4: HEADERS ==============
// Self-contained header: has header guards and includes all headers it needs.
// Any .cc file that includes this can rely on it compiling without extra includes.
// Google C++ Style: .h extension, no #pragma once; inline/template defs stay in .h.

#ifndef START_SENSOR_H_
#define START_SENSOR_H_

#include <string>  // std::string (used in declarations below)

// Declarations only. Definitions in sensor.cpp.

// Interface: "something that can read a frame" (e.g. camera, dataset, simulator).
class Sensor {
public:
    virtual ~Sensor() = default;
    virtual std::string name() const = 0;
    virtual int read_frame() = 0;
};

class CameraSensor final : public Sensor {
public:
    std::string name() const override;
    int read_frame() override;

private:
    static int next_frame_id_;
};

class DummySensor final : public Sensor {
public:
    std::string name() const override;
    int read_frame() override;
};

void use_sensor(Sensor& s);
void abstract_class_examples();

#endif  // START_SENSOR_H_
