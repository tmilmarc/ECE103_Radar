cmake_minimum_required(VERSION 3.20)

project(ECE103_Radar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Links all cpp files in src folder to make the executable
add_executable(ECE103_Radar
    src/main.cpp
    src/Application.cpp
    src/Renderer.cpp
    src/Radar.cpp
    src/SerialPort.cpp
)

target_include_directories(ECE103_Radar PRIVATE
    include
)
