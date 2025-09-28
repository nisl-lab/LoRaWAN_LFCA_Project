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
project(lora-mac)
cmake_minimum_required(VERSION 3.6)

#---------------------------------------------------------------------------------------
# Options
#---------------------------------------------------------------------------------------

# Allow switching of target platform
set(BOARD_LIST NucleoL476)
set(BOARD NucleoL476 CACHE STRING "Default target platform is NucleoL476")
set_property(CACHE BOARD PROPERTY STRINGS ${BOARD_LIST})

# Allow switching of MBED shields
set(MBED_RADIO_SHIELD_LIST SX1276MB1LAS )
set(MBED_RADIO_SHIELD SX1276MB1LAS CACHE STRING "Default MBED radio shield is SX1276MB1LAS")
set_property(CACHE MBED_RADIO_SHIELD PROPERTY STRINGS ${MBED_RADIO_SHIELD_LIST})

# Allow switching of secure-elements
set(SECURE_ELEMENT_LIST SOFT_SE)
set(SECURE_ELEMENT SOFT_SE CACHE STRING "Default secure element is SOFT_SE")
set_property(CACHE SECURE_ELEMENT PROPERTY STRINGS ${SECURE_ELEMENT_LIST})

# Allow switching of Applications
set(APPLICATION_LIST LoRaMac )
set(APPLICATION LoRaMac CACHE STRING "Default Application is LoRaMac")
set_property(CACHE APPLICATION PROPERTY STRINGS ${APPLICATION_LIST})

# Switch for USB-Uart support, enable it for some Applications who needs it.
option(USE_USB_CDC "Use USB-Uart" OFF)

# Switch for debugger support.
option(USE_DEBUGGER "Use Debugger" ON)

# Switch for Class B support of LoRaMac.
option(CLASSB_ENABLED "Class B support of LoRaMac" OFF)

#---------------------------------------------------------------------------------------
# Target Boards
#---------------------------------------------------------------------------------------
# Configure toolchain for NucleoL476
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/boards/NucleoL476/cmsis/arm-gcc/stm32l476rgtx_flash.ld)
include(stm32l4)

# Build platform specific board implementation
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/boards/NucleoL476)
set(RADIO sx1276 CACHE INTERNAL "Radio sx1276 selected")


#---------------------------------------------------------------------------------------
# General Components
#---------------------------------------------------------------------------------------

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/boards)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/radio)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/system)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mac)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/peripherals)

#---------------------------------------------------------------------------------------
# Applications
#---------------------------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/apps/LoRaMac)

