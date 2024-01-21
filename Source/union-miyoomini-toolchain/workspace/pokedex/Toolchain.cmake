# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# sysroot location
set(SYSROOT /opt/miyoo/arm-buildroot-linux-gnueabihf/sysroot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/lib/gcc/x86_64-linux-gnu/8/include/")

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH "${SYSROOT}")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)