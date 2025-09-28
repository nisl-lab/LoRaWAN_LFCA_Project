##
##   ______                              _
##  / _____)             _              | |
## ( (____  _____ ____ _| |_ _____  ____| |__
##  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
##  _____) ) ____| | | || |_| ____( (___| | | |
## (______/|_____)_|_|_| \__)_____)\____)_| |_|
## (C)2013-2017 Semtech
##  ___ _____ _   ___ _  _____ ___  ___  ___ ___
## / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
## \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
## |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
## embedded.connectivity.solutions.==============
##
## License:  Revised BSD License, see LICENSE.TXT file included in the project
## Authors:  Johannes Bruder (STACKFORCE), Miguel Luis (Semtech)
##
# CMake 버전 및 툴체인 설정
cmake_minimum_required(VERSION 3.6)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-arm-none-eabi.cmake)

project(loramac-node)

# CPU 및 FPU 설정
set(CPU_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")

# 빌드 유형에 따른 컴파일러 플래그 설정
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 ${CPU_FLAGS} -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32L476xx")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 ${CPU_FLAGS} -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32L476xx")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 ${CPU_FLAGS} -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32L476xx")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 ${CPU_FLAGS} -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32L476xx")
endif()

# 부동소수점 지원을 위한 링커 플래그 추가
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _printf_float")

# 소스 디렉토리 추가
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)