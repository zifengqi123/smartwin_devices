cmake_minimum_required(VERSION 3.10)
project(smartwin_devices)

# Usage:
# cmake -B build -S .
# make  -C build -j

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(tools /usr/local/toolchain/linux64)
set(CMAKE_C_COMPILER ${tools}/bin/arm-openwrt-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-openwrt-linux-gnueabi-g++)

# If necessary, set STAGING_DIR
# if not work, please try(in shell command): export STAGING_DIR=/home/ubuntu/Your_SDK/out/xxx/openwrt/staging_dir/target
#set(ENV{STAGING_DIR} "/home/ubuntu/Your_SDK/out/xxx/openwrt/staging_dir/target")

# Uncomment if the program needs debugging
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")

set(CMAKE_C_STANDARD 99) # LVGL officially supports C99 and above
set(CMAKE_CXX_STANDARD 17) #C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


include_directories(
    include
    include/serial
    )

add_library(smartwin_devices SHARED 
    ${PROJECT_SOURCE_DIR}/src/smartwin_devices.cpp
    ${PROJECT_SOURCE_DIR}/src/smartwin_comm.cpp
    ${PROJECT_SOURCE_DIR}/src/serial/serial.cpp
    ${PROJECT_SOURCE_DIR}/src/serial/list_ports_linux.cpp
    ${PROJECT_SOURCE_DIR}/src/serial/unix.cpp
)

add_executable(smartwin_test 
    test/smartwin_test.cpp
)

target_link_libraries(smartwin_test 
    smartwin_devices
    pthread 
)

install(DIRECTORY include
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h")

install(TARGETS smartwin_devices
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

