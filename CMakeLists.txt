cmake_minimum_required (VERSION 2.8.8)

project(avr-net C CXX)
set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})


set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)

# Setup the Arduino SDK
#-----------------------------------------------------------------------

set(ARDUINO_SDK_PATH /Applications/Arduino.app/Contents/Resources/Java)
set(SERIAL_PORT /dev/tty.usbserial-A600af0m)
set(ARDUINO_BOARD uno)

include_directories(./include)


# AVRNET core
include_directories(./src/include)
set(SRC ${SRC}
    ./src/uip_callback.c
    ./src/core.cc
)


# UIP
include_directories(./src/uip)
set(SRC ${SRC}
  ./src/uip/psock.c
  ./src/uip/timer.c
  ./src/uip/uip-fw.c
  ./src/uip/uip-neighbor.c
  ./src/uip/uip-split.c
  ./src/uip/uip.c
  ./src/uip/uip_arp.c
  ./src/uip/uiplib.c
)


# ARV
include_directories(./src/hardware/cpu/avr)
set(SRC ${SRC}
./src/hardware/cpu/avr/clock-arch.c
)


# Serial
include_directories(./src/hardware/uart)
set(SRC ${SRC}
  ./src/hardware/uart/uart.c
)


# Ethernet
include_directories(./src/hardware/net/enc28j60)
set(SRC ${SRC}
  ./src/hardware/net/enc28j60/enc28j60.c
  ./src/hardware/net/enc28j60/network.c
)


# Create the shared library

generate_arduino_library(avr-net
  SRCS ${SRC}
  BOARD ${ARDUINO_BOARD}
  NO_AUTOLIBS
)
link_directories(./build)

generate_arduino_firmware(test
  SRCS ./src/main.cc
  LIBS avr-net
  BOARD ${ARDUINO_BOARD}
  PORT ${SERIAL_PORT}
  NO_AUTOLIBS
)