cmake_minimum_required(VERSION 3.16)

project(VoleGUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# The core files live in the parent folder (they are NOT copied or modified).
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

qt_add_executable(VoleGUI
    main.cpp
    mainwindow.cpp
    mainwindow.h
    ${CORE_DIR}/CPU.cpp
    ${CORE_DIR}/ALU.cpp
    ${CORE_DIR}/Instructions.cpp
    ${CORE_DIR}/Registers.cpp
    ${CORE_DIR}/Memory.cpp
    ${CORE_DIR}/ProgramLoader.cpp
)

target_include_directories(VoleGUI PRIVATE ${CORE_DIR})
target_link_libraries(VoleGUI PRIVATE Qt6::Widgets)
