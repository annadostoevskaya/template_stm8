cmake_minimum_required(VERSION 3.5)
set(CMAKE_TOOLCHAIN_FILE "SDCCToolchain.cmake")

project(blinky C)

set(CMAKE_EXPORT_COMPILER_COMMANDS ON)

include_directories("./src")
file(GLOB SRC "./src/*.c")

set(CMAKE_C_FLAGS "-lstm8 -mstm8 --out-fmt-ihx --std=c99")

add_executable(${PROJECT_NAME} ${SRC})

add_custom_target(hex   ALL packihx ${PROJECT_NAME}.ihx > ${PROJECT_NAME}.hex DEPENDS ${PROJECT_NAME})
add_custom_target(flash     stm8flash -c stlinkv2 -p stm8s003f3 -w ${PROJECT_NAME}.hex    DEPENDS hex)

