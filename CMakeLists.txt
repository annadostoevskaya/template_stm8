cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_TOOLCHAIN_FILE SDCC.toolchain.cmake)

project(blinky C)

include_directories("./src")
file(GLOB SRC "./src/*.c")

set(CMAKE_C_FLAGS "-mstm8 --out-fmt-ihx --std=c99 --verbose")

add_executable(${PROJECT_NAME} ${SRC})
add_definitions(-D__SDCC_stm8 -DF_CPU=2000000UL)

add_custom_target(hex   ALL packihx ${PROJECT_NAME}.ihx > ${PROJECT_NAME}.hex DEPENDS ${PROJECT_NAME})
add_custom_target(flash     stm8flash -c stlinkv2 -p stm8s003f3 -w ${PROJECT_NAME}.hex    DEPENDS hex)

