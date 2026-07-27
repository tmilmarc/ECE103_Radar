cmake_minimum_required(VERSION 3.20)

project(ECE103_Radar)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL3_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL3/SDL3-3.4.12/cmake")

find_package(SDL3 REQUIRED CONFIG)

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

target_link_libraries(ECE103_Radar PRIVATE
    SDL3::SDL3-shared
)

add_custom_command(TARGET ECE103_Radar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_DIR}/../lib/arm64/SDL3.dll"
        $<TARGET_FILE_DIR:ECE103_Radar>)