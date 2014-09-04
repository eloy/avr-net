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
set(INSTALLABLE_HEADERS ${INSTALLABLE_HEADERS}
  ./include/avrnet.h
  ./include/network.h
  ./include/app.h
  ./src/include/uip_callback.h
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
set(INSTALLABLE_HEADERS ${INSTALLABLE_HEADERS}
  ./src/uip/clock.h
  ./src/uip/lc-addrlabels.h
  ./src/uip/lc-switch.h
  ./src/uip/lc.h
  ./src/uip/psock.h
  ./src/uip/pt.h
  ./src/uip/timer.h
  ./src/uip/uip-fw.h
  ./src/uip/uip-neighbor.h
  ./src/uip/uip-split.h
  ./src/uip/uip.h
  ./src/uip/uip_arch.h
  ./src/uip/uip_arp.h
  ./src/uip/uiplib.h
  ./src/uip/uipopt.h
)


# ARV
include_directories(./src/hardware/cpu/avr)
set(SRC ${SRC}
./src/hardware/cpu/avr/clock-arch.c
)
set(INSTALLABLE_HEADERS ${INSTALLABLE_HEADERS}
  ./src/hardware/cpu/avr/uip-conf.h
  ./src/hardware/cpu/avr/clock-arch.h
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

# generate_arduino_firmware(test
#   SRCS ./src/main.cc
#   LIBS avr-net
#   BOARD ${ARDUINO_BOARD}
#   PORT ${SERIAL_PORT}
#   NO_AUTOLIBS
# )

# Install Files
install (FILES ${INSTALLABLE_HEADERS} DESTINATION include)