# Test executable links the library (gets include dir via PUBLIC).
add_executable(test_sensor test_sensor.cpp)
target_link_libraries(test_sensor PRIVATE start_lib)
target_compile_options(test_sensor PRIVATE -Wall -Wextra -Wpedantic)

add_test(NAME test_sensor COMMAND test_sensor)
