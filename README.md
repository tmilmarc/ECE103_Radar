#ECE103_Radar

##Overview

##Design Goals

##Hardware

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





