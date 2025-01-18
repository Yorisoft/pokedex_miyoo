cmake_minimum_required(VERSION 3.10)
project(retrodex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR /root/workspace/retrodex)

set(SDL2_INCLUDE_DIRS /root/workspace/build/include)
set(SDL2_LIBRARIES /root/workspace/build/lib/libSDL2.so)
set(SDL2_IMAGE_LIBRARIES /root/workspace/build/lib/libSDL2_image.so)
set(SDL2_TTF_LIBRARIES /root/workspace/build/lib/libSDL2_ttf.so)
set(SDL2_MIXER_LIBRARIES /root/workspace/build/lib/libSDL2_mixer.so)

set(SQLITE_INCLUDE_DIRS ${SOURCE_DIR}/sqlite)
set(SQLITE_SOURCE ${SOURCE_DIR}/sqlite/sqlite3.c)

include_directories(${SDL2_INCLUDE_DIRS} ${SOURCE_DIR} ${SQLITE_INCLUDE_DIRS})

# Find all .cpp and .cc files in the source directory
file(GLOB SOURCES ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.cc)

add_executable(retrodex ${SOURCES} ${SQLITE_SOURCE})

# link the libraries based on our above
target_link_libraries(retrodex 
	${SDL2_LIBRARIES} 
	${SDL2_IMAGE_LIBRARIES} 
	${SDL2_TTF_LIBRARIES} 
	${SDL2_MIXER_LIBRARIES} 
	${CMAKE_DL_LIBS} 
	-pthread 
	stdc++fs
)

install(TARGETS retrodex DESTINATION /opt/miyoo/arm-buildroot-linux-gnueabihf/sysroot/build)
