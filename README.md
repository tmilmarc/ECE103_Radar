#ECE103_Radar

##Overview
Our goal with this project is to create a working radar detection system using an ultrasonic sensor in conjunction with a micro servo motor. The idea is to have the ultrasonic motor spin on the servo motor and aim towards the detected object. Our final end goal is to use a laser as well that would work as a faux laser-guided defense system.
##Design Goals

##Hardware
-Huzzah ESP32 Micro Controller
-Two Micro Servo 9g Motors
-Breadboard
-Ultrasonic Sensor
-Laser Diode

##Software
-Language: C++
-Graphics Library: SDL3
-Build System: Cmake

Embedded Code
-Platform: Aurdiuno IDE
-Language C++
-File: radar.ino

##Project Structure
ECE103_Radar/
|
├── src/
| ├── main.cpp
| ├── Application.cpp
| ├── Renderer.cpp
| ├── Radar.cpp
| └── SerialPort.cpp
|
├── include/
| ├── Application.h
| ├── Renderer.h
| ├── Radar.h
| └── SerialPort.h
|
├── firmware/
| └── radar.ino
|
├── third_party/
| └── SDL3/
|
├── build/
|
├── CMakeLists.txt
└── README.md

##Building

Requirements:
-CMake
-C++ compiler
-SDL3

Configure the project:
cmake -B build
cmake -build build --config Release



##Running

After build:
.\build\Release\ECE103_Radar.exe

##ESP32 communication

The ESP32 sends radar data over serial

Current data format:
-angle,distance





