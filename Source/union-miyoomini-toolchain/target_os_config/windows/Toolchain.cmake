# Specify the system name
SET(CMAKE_SYSTEM_NAME Windows)

# Specify the system version (optional)
SET(CMAKE_SYSTEM_VERSION 10)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Specify the cross compiler (adjust to your compiler's name)
set(CMAKE_C_COMPILER ${SYSROOT}/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER ${SYSROOT}/bin/x86_64-w64-mingw32-g++)

# Sysroot location (update if cross-compiling)
# You can remove this if you're compiling natively on WSL2 without cross-compilation
set(SYSROOT /opt/mingw-w64/x86_64-w64-mingw32)
SET(CMAKE_FIND_ROOT_PATH "${SYSROOT}")

# Specify the necessary include directories for the cross-compilation
set(CMAKE_C_FLAGS "-I${SYSROOT}/include" CACHE STRING "Additional C flags")
set(CMAKE_CXX_FLAGS "-I${SYSROOT}/include" CACHE STRING "Additional C++ flags")
set(CMAKE_EXE_LINKER_FLAGS "-L${SYSROOT}/lib" CACHE STRING "Additional linker flags")

# Set up the location of binutils for the target system
set(CMAKE_AR ${SYSROOT}/bin/x86_64-w64-mingw32-ar)
set(CMAKE_AS ${SYSROOT}/bin/x86_64-w64-mingw32-as)
set(CMAKE_RANLIB ${SYSROOT}/bin/x86_64-w64-mingw32-ranlib)
set(CMAKE_LD ${SYSROOT}/bin/x86_64-w64-mingw32-ld)

# Search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_CROSSCOMPILING TRUE)
