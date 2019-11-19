cmake_minimum_required(VERSION 2.8.3)
project(tdoa_code)

add_compile_options("-fpermissive")


############### FOR TDOA #################
link_directories(/usr/local/lib/libliquid.so)
link_directories(/usr/local/lib/libbcm2835.a)
link_directories(/usr/lib)
include_directories(include)

add_library(tdoa include/Util.h include/tdoa.h src/tdoa.cpp src/Util.cpp)

add_executable(server src/server.cpp src/Util.cpp)
add_executable(debug src/debug.cpp src/Util.cpp)
add_executable(rtdoa src/rtdoa.cpp src/Util.cpp)

#file(GLOB SOURCES "src/*.cpp")
#add_executable(exe ${SOURCES})
#target_link_libraries(exe liquid bcm2835 fftw3)

target_link_libraries(server liquid bcm2835 fftw3)
target_link_libraries(debug liquid bcm2835 fftw3)
target_link_libraries(rtdoa liquid bcm2835 fftw3)
