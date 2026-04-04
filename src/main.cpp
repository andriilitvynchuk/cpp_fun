#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <variant>
#include <vector>

#include "start/sensor.h"

void print_name(const std::string& name) { std::cout << "My name is " << name << std::endl; }

// ============== Step 1: TEMPLATES ==============
// Templates = code parameterized by type (or value). Compiler generates
// concrete code per type. Used everywhere: std::vector<T>, cv::Mat,
// Eigen::Matrix<T,R,C>.

// Function template: one implementation, many types
template <typename T>
T clamp(T value, T lo, T hi) {
  if (value < lo) return lo;
  if (value > hi) return hi;
  return value;
}

// Class template: e.g. buffer of any element type (like cv::Mat channels, or
// landmark type in SLAM)
template <typename T>
class Buffer {
 public:
  explicit Buffer(size_t size) : data_(size) {}
  T& at(size_t i) { return data_.at(i); }
  const T& at(size_t i) const { return data_.at(i); }
  size_t size() const { return data_.size(); }

 private:
  std::vector<T> data_;
};

// ============== Step 2: ERROR HANDLING ==============
// Two main tools: exceptions (for "this should never happen" / unrecoverable)
// and std::optional / std::expected (for "no result" that callers can handle).

// Custom exception: useful for domain errors (e.g. SLAM: tracking lost, no
// pose).
struct TrackingLostError : std::runtime_error {
  explicit TrackingLostError(const std::string& msg) : std::runtime_error(msg) {}
};

// Throw when precondition is violated (programming error or invalid input).
double safe_divide(double a, double b) {
  if (b == 0.0) throw std::invalid_argument("safe_divide: divisor is zero");
  return a / b;
}

// Return optional when "no result" is a valid outcome (e.g. keyframe not
// found).
std::optional<int> find_index(const std::vector<int>& v, int value) {
  for (size_t i = 0; i < v.size(); ++i)
    if (v[i] == value) return static_cast<int>(i);
  return std::nullopt;
}

void error_handling_examples() {
  // Optional: normal control flow, no exception
  std::vector<int> vec = {10, 20, 30};
  auto idx = find_index(vec, 20);
  if (idx.has_value())
    std::cout << "found at " << idx.value() << std::endl;
  else
    std::cout << "not found" << std::endl;
  std::cout << "index of 99: " << find_index(vec, 99).value_or(-1) << std::endl;

  // Exceptions: catch by reference, handle or rethrow
  try {
    safe_divide(1.0, 0.0);
  } catch (const std::invalid_argument& e) {
    std::cout << "caught: " << e.what() << std::endl;
  }

  try {
    throw TrackingLostError("no features in frame");
  } catch (const std::exception& e) {
    std::cout << "tracking error: " << e.what() << std::endl;
  }
}

// Step 3 (abstract classes) live in include/start/sensor.h and src/sensor.cpp.

// ============== Step 5: RECOMMENDATIONS FOR OPENCV / GTSAM / SLAM
// ==============
// - RAII: tie resources to object lifetime; no raw new/delete (use smart
// pointers, containers).
// - Rule of 0/3/5: if you need custom dtor/copy/move, define all that are
// needed; else = default or omit.
// - Namespaces: use your own (e.g. slam_demo::) to avoid clashes with cv::,
// std::.
// - Const ref for big types: pass cv::Mat, Eigen::Matrix by const ref when
// read-only.
// - Config structs: named params for system/frontend/backend (you already saw
// this).

namespace slam_demo {

struct TrackerConfig {
  int max_features = 200;
  double scale_factor = 1.2;
  bool use_rotation = true;
};

// RAII: resource (here just "scope") tied to object lifetime. Like cv::Mat
// releasing memory in dtor.
class ScopeTrace {
 public:
  explicit ScopeTrace(const std::string& label) : label_(label) { std::cout << "[enter] " << label_ << std::endl; }
  ~ScopeTrace() { std::cout << "[exit] " << label_ << std::endl; }
  ScopeTrace(const ScopeTrace&) = delete;
  ScopeTrace& operator=(const ScopeTrace&) = delete;

 private:
  std::string label_;
};

void run_tracker(const TrackerConfig& cfg) {
  ScopeTrace trace("run_tracker");
  (void)cfg;
  // In real code: use cfg.max_features, cfg.scale_factor, etc.
}

void step5_examples() {
  TrackerConfig cfg;
  cfg.max_features = 500;
  run_tracker(cfg);
  run_tracker(TrackerConfig{.use_rotation = false});  // designated init
}

}  // namespace slam_demo

class Learner {
 private:
  std::string private_field_;
  std::vector<int> pixels_;

 public:
  Learner(std::string private_field) : private_field_(private_field) {}
  Learner(std::string private_field, std::vector<int> pixels) : private_field_(private_field), pixels_(pixels) {}
  ~Learner() { std::cout << "Learner destructor" << std::endl; }

  std::string get_private_field() { return private_field_; }
  std::vector<int> get_pixels() { return pixels_; }

  void unique_ptr_examples() {
    auto int_array_pointer = std::unique_ptr<int[]>(new int[10]);
    for (int i = 0; i < 10; i++) {
      int_array_pointer[i] = i;
      std::cout << int_array_pointer[i] << std::endl;
    }

    auto int_pointer = std::shared_ptr<int>(new int(10));
    std::cout << *int_pointer << std::endl;
    auto int_pointer_2 = int_pointer;
    std::cout << *int_pointer_2 << std::endl;
  }
};

class Image {
 public:
  std::vector<int> pixels;

  Image(std::vector<int> p) : pixels(std::move(p)) {}

  Image(const Image& other) {
    pixels = other.pixels;
    std::cout << "copied\n";
  }

  Image(Image&& other) {
    pixels = std::move(other.pixels);
    std::cout << other.pixels.size() << std::endl;
    std::cout << "moved\n";
  }
};

class STL {
 public:
  void stl_examples() {
    std::vector<int> pixels = {4, 2, 1, 3};
    std::cout << pixels.size() << std::endl;

    std::sort(pixels.begin(), pixels.end(), [](int& a, int& b) { return a > b; });
    for (auto& pixel : pixels) {
      std::cout << pixel << std::endl;
    }
  }
  std::optional<int> optional_example(bool b) {
    if (b) {
      return true;
    }
    return std::nullopt;
  };

  void ranges_examples() {
    auto vector = std::vector<int>({1, 2, 3, 4, 5});
    auto result = vector | std::ranges::views::filter([](int& element) { return element % 2 == 0; }) |
                  std::ranges::views::transform([](int& element) { return element * 2; });
    for (auto element : result) {
      std::cout << element << " ";
    }
    std::cout << std::endl;
  }
};

int main() {
  std::string name = "Andrii & Kate L";
  print_name(name);

  auto learner = Learner(name);
  std::cout << learner.get_private_field() << std::endl;

  auto learner_pointer = std::unique_ptr<Learner>(new Learner(name));
  std::cout << learner_pointer << learner_pointer->get_private_field() << std::endl;

  learner_pointer->unique_ptr_examples();

  auto learner2 = Learner("kek");

  auto image1 = Image({1, 2, 3});
  auto image3 = std::move(image1);  // move constructor
  auto image2 = image1;             // copy constructor
  std::cout << image1.pixels.size() << std::endl;
  std::cout << image2.pixels.size() << std::endl;
  std::cout << image3.pixels.size() << std::endl;

  auto stl = STL();
  stl.stl_examples();

  auto result = stl.optional_example(true);
  std::cout << result.value() << std::endl;
  auto result2 = stl.optional_example(false);
  if (result2.has_value()) {
    std::cout << result2.value() << std::endl;
  } else {
    std::cout << "no value" << std::endl;
  }
  std::cout << result2.value_or(0) << std::endl;

  std::variant<int, std::string> variant = "10";
  if (std::holds_alternative<int>(variant)) {
    std::cout << std::get<int>(variant) << std::endl;
  } else {
    std::cout << "no int value" << std::endl;
  }
  std::visit([](auto&& arg) { std::cout << arg << "\n"; }, variant);

  // Step 1: Templates
  std::cout << clamp(10, 0, 5) << " " << clamp(3.14, 0.0, 1.0) << std::endl;  // 5, 1
  Buffer<int> ibuf(3);
  ibuf.at(0) = 1;
  ibuf.at(1) = 2;
  ibuf.at(2) = 3;
  Buffer<double> dbuf(2);
  dbuf.at(0) = 0.5;
  dbuf.at(1) = 1.5;
  std::cout << "Buffer<int> size " << ibuf.size() << ", Buffer<double> size " << dbuf.size() << std::endl;

  // Step 2: Error handling
  error_handling_examples();

  // Step 3: Abstract classes & interfaces
  abstract_class_examples();

  // Step 5: RAII, namespaces, config structs (OpenCV/SLAM style)
  slam_demo::step5_examples();

  stl.ranges_examples();
  return 0;
}
