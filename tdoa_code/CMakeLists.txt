cmake_minimum_required(VERSION 2.8.3)
project(tdoa_code)

link_directories(/usr/local/lib/libliquid.so)
link_directories(/usr/local/lib/libbcm2835.a)
link_directories(/usr/lib)
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(exe ${SOURCES})
 target_link_libraries(exe liquid bcm2835 fftw3)
# add_dependencies(exe gazebo_node_generate_messages_cpp)
