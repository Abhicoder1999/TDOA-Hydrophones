cmake_minimum_required(VERSION 2.8.3)
project(tdoa_code)

add_compile_options("-fpermissive")

############# FOR GUI ###################

#find_package(
#  catkin REQUIRED COMPONENTS
#  qt_build
#  std_msgs
#)

find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
set(QT_MOC include/hydrophones.h include/qcustomplot.h)
SET(QT_SOURCES src/hydrophones.cpp src/qcustomplot.cpp)

QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

include_directories(include)
include_directories(
	${QT_INCLUDE_DIRS}
)

add_library(hydrophones_lib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(hydrophones_lib ${QT_LIBRARIES})

add_executable(hydrophones src/main.cpp)
target_link_libraries(hydrophones hydrophones_lib)

############### FOR TDOA #################
link_directories(/usr/local/lib/libliquid.so)
link_directories(/usr/local/lib/libbcm2835.a)
link_directories(/usr/lib)
include_directories(include)

add_library(tdoa_lib include/Util.h include/tdoa.h src/Util.cpp ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(tdoa_lib ${QT_LIBRARIES})

#add_executable(server src/server.cpp src/Util.cpp src/hydrophones.cpp)
add_executable(debug src/debug.cpp src/Util.cpp src/hydrophones.cpp)
add_executable(rtdoa src/rtdoa.cpp src/Util.cpp src/hydrophones.cpp)

#file(GLOB SOURCES "src/*.cpp")
#add_executable(exe ${SOURCES})
#target_link_libraries(exe liquid bcm2835 fftw3)

#target_link_libraries(server liquid bcm2835 fftw3 tdoa_lib)
target_link_libraries(debug liquid bcm2835 fftw3 tdoa_lib)
target_link_libraries(rtdoa liquid bcm2835 fftw3 tdoa_lib)
