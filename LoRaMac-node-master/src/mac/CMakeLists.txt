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
project(mac)
cmake_minimum_required(VERSION 3.6)

#---------------------------------------------------------------------------------------
# Options
#---------------------------------------------------------------------------------------

# Allow selection of region
option(REGION_KR920 "Region KR920" ON)
set(REGION_LIST REGION_KR920 )


#---------------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------------
set( MAC_BUILD_SOURCES
     ${CMAKE_CURRENT_SOURCE_DIR}/region/RegionCommon.c
     ${CMAKE_CURRENT_SOURCE_DIR}/region/Region.c
     ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac.c
     ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMacAdr.c
     ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMacClassB.c
     ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMacCommands.c
     ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMacConfirmQueue.c
     ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMacCrypto.c
     ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMacParser.c
     ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMacSerializer.c )

set( MAC_BUILD_SOURCES ${MAC_BUILD_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/region/RegionKR920.c" )
add_library(${PROJECT_NAME} OBJECT EXCLUDE_FROM_ALL ${MAC_BUILD_SOURCES})

# Loops through all regions and add compile time definitions for the enabled ones.
foreach( REGION ${REGION_LIST} )
    if(${REGION})
        target_compile_definitions(${PROJECT_NAME} PUBLIC -D"${REGION}")
    endif()
endforeach()


# SecureElement NVM
if(${SECURE_ELEMENT} MATCHES SOFT_SE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DSOFT_SE)
endif()

add_dependencies(${PROJECT_NAME} board)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/region
    $<TARGET_PROPERTY:system,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:radio,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:board,INTERFACE_INCLUDE_DIRECTORIES>
)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
