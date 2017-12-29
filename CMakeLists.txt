cmake_minimum_required(VERSION 3.7)
project(RaspberryPiSockets)

set(CMAKE_CXX_STANDARD 11)


add_definitions(-DPONG)

include_directories(lib/mqtt-sn-sockets)
include_directories(lib/mqtt-sn-sockets/lib)
#include_directories(lib/mqtt-sn-sockets/lib/arduino-linux-abstraction/src)
#add_subdirectory(lib/mqtt-sn-sockets/lib/arduino-linux-abstraction/)
add_subdirectory(lib/mqtt-sn-sockets)

set(RH95_SOURCE_FILES src/main.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RHDatagram.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RHDatagram.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericDriver.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericDriver.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericSPI.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RHGenericSPI.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RHLinuxSPI.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RHLinuxSPI.cpp
		lib/mqtt-sn-sockets/RadioHeadRpi/RH_RF95.h
		lib/mqtt-sn-sockets/RadioHeadRpi/RH_RF95.cpp
		lib/mqtt-sn-sockets/lib/MqttSnMessageHandler.cpp
#                lib/mqtt-sn-sockets/lib/SimpleMqttSnClientTester.cpp
)


add_executable(PIRH95Main ${RH95_SOURCE_FILES})

target_compile_definitions(PIRH95Main PRIVATE -DRH_PLATFORM=RH_PLATFORM_RPI)
#target_compile_definitions(PIRH95Main PRIVATE -DSIMPLEMQTTSNCLIENTTESTER)
target_compile_definitions(PIRH95Main PRIVATE -DDRIVER_RH_RF95)
target_compile_definitions(PIRH95Main PRIVATE -DFREQUENCY=868)
target_compile_definitions(PIRH95Main PRIVATE -DMODEM_CONFIG_CHOICE=Bw31_25Cr48Sf512)

target_include_directories(PIRH95Main PRIVATE lib/mqtt-sn-sockets/RadioHeadRpi)

target_link_libraries(PIRH95Main example-lib-rh96-socket wiringPi)


set(RH_NRF24_SOURCE_FILES src/main.cpp
		lib/mqtt-sn-sockets/RadioHead/RHutil/RasPi.cpp
		lib/mqtt-sn-sockets/RadioHead/RH_NRF24.cpp
                lib/mqtt-sn-sockets/RadioHead/RHDatagram.cpp

		lib/mqtt-sn-sockets/RadioHead/RHReliableDatagram.cpp
		lib/mqtt-sn-sockets/RadioHead/RHDatagram.cpp
		lib/mqtt-sn-sockets/RadioHead/RHHardwareSPI.cpp
		lib/mqtt-sn-sockets/RadioHead/RHNRFSPIDriver.cpp

                lib/mqtt-sn-sockets/RadioHead/RHGenericDriver.cpp
                lib/mqtt-sn-sockets/RadioHead/RHGenericSPI.cpp

                lib/mqtt-sn-sockets/lib/MqttSnMessageHandler.cpp
#                lib/mqtt-sn-sockets/lib/SimpleMqttSnClientTester.cpp
)

add_executable(PIRH24Main ${RH_NRF24_SOURCE_FILES})

target_compile_definitions(PIRH24Main PRIVATE -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY)
target_compile_definitions(PIRH24Main PRIVATE -DDRIVER_RH_NRF24)

target_include_directories(PIRH24Main PRIVATE lib/mqtt-sn-sockets/RadioHead lib/mqtt-sn-sockets/RadioHead/RHutil)

target_link_libraries(PIRH24Main example-lib-rh-nrf24-socket bcm2835)


