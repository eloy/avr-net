# set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
cmake_minimum_required (VERSION 2.8.8)
project(uip C CXX)
set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Setup the Arduino SDK
#-----------------------------------------------------------------------

set(ARDUINO_SDK_PATH /Applications/Arduino.app/Contents/Resources/Java)
set(SERIAL_PORT /dev/tty.usbserial-A600af0m)
set(ARDUINO_BOARD uno)

include_directories(./include)

# UIP
include_directories(./uip)
set(SRC ${SRC}
  ./uip/psock.c
  ./uip/timer.c
  ./uip/uip-fw.c
  ./uip/uip-neighbor.c
  ./uip/uip-split.c
  ./uip/uip.c
  ./uip/uip_arp.c
  ./uip/uiplib.c
  ./uip/uip_callback.c
)



# ARV
# set(CMAKE_C_FLAGS "-mmcu=")
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

# Net app
# include_directories(./apps/net)
# set(SRC ${SRC}
#   ./apps/net/hello-world.c
# )


# Create the shared library
# add_library (uip SHARED ${SRC})
# target_link_libraries (uip ${DEP_LIBS})

generate_arduino_library(avr-net
  SRCS ${SRC}
  BOARD ${ARDUINO_BOARD}
)
link_directories(./build)

generate_arduino_firmware(test
  SRCS ./src/main.cc
  LIBS avr-net
  BOARD ${ARDUINO_BOARD}
  PORT ${SERIAL_PORT}
)