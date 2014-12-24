
cmake_minimum_required(VERSION 2.6)
project(CC2540)

## Target
set(CC2540_HEADERS
    src/BLE.h
    src/BLEDevice.h
    src/AsyncSerial.h
    src/HCICodes.h
    src/Singleton.h
)
set(CC2540_SRCS
    src/test.cpp
    src/AsyncSerial.cpp
    src/BLE.cpp
    src/BLEDevice.cpp
)

add_executable(CC2540 ${CC2540_SRCS} ${CC2540_HEADERS})

## Link libraries
set(BOOST_LIBS thread date_time system)
find_package(Boost COMPONENTS ${BOOST_LIBS} REQUIRED)
target_link_libraries(CC2540 ${Boost_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(CC2540 ${CMAKE_THREAD_LIBS_INIT})
