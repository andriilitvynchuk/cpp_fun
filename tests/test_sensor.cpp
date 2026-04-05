#include <memory>

#include "gtest/gtest.h"
#include "start/sensor.h"

// ---- DummySensor ----

TEST(DummySensorTest, Name) {
  DummySensor dummy;
  EXPECT_EQ(dummy.name(), "Dummy");
}

TEST(DummySensorTest, ReadFrameAlwaysReturnsMinusOne) {
  DummySensor dummy;
  EXPECT_EQ(dummy.read_frame(), -1);
  EXPECT_EQ(dummy.read_frame(), -1);
  EXPECT_EQ(dummy.read_frame(), -1);
}

// ---- CameraSensor ----
// CameraSensor::next_frame_id_ is a static class variable shared across all
// instances and tests. We can't rely on absolute values; test relative
// behaviour instead.

TEST(CameraSensorTest, Name) {
  CameraSensor cam;
  EXPECT_EQ(cam.name(), "Camera");
}

TEST(CameraSensorTest, ReadFrameIncrementsSequentially) {
  CameraSensor cam;
  const int first = cam.read_frame();
  EXPECT_EQ(cam.read_frame(), first + 1);
  EXPECT_EQ(cam.read_frame(), first + 2);
}

TEST(CameraSensorTest, IndependentInstancesShareCounter) {
  CameraSensor cam_a;
  CameraSensor cam_b;
  const int a0 = cam_a.read_frame();
  const int b0 = cam_b.read_frame();
  // Both cameras pull from the same global counter.
  EXPECT_EQ(b0, a0 + 1);
}

// ---- Polymorphism via Sensor interface ----

TEST(SensorInterfaceTest, CameraIsASensor) {
  CameraSensor cam;
  Sensor& s = cam;
  EXPECT_EQ(s.name(), "Camera");
}

TEST(SensorInterfaceTest, DummyIsASensor) {
  DummySensor dummy;
  Sensor& s = dummy;
  EXPECT_EQ(s.name(), "Dummy");
  EXPECT_EQ(s.read_frame(), -1);
}

TEST(SensorInterfaceTest, UniquePtrPolymorphism) {
  std::unique_ptr<Sensor> cam = std::make_unique<CameraSensor>();
  std::unique_ptr<Sensor> dummy = std::make_unique<DummySensor>();

  EXPECT_EQ(cam->name(), "Camera");
  EXPECT_EQ(dummy->name(), "Dummy");
  EXPECT_EQ(dummy->read_frame(), -1);
}

TEST(SensorInterfaceTest, SensorNamesAreDistinct) {
  CameraSensor cam;
  DummySensor dummy;
  EXPECT_NE(cam.name(), dummy.name());
}
