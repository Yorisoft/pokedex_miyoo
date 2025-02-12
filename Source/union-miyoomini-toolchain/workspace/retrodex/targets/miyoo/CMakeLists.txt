cmake_minimum_required(VERSION 3.10)
project(retrodex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL2_INCLUDE_DIRS /root/workspace/build/include)

set(SDL2_LIBRARIES /root/workspace/build/lib/libSDL2.so)
set(SDL2_IMAGE_LIBRARIES /root/workspace/build/lib/libSDL2_image.so)
set(SDL2_TTF_LIBRARIES /root/workspace/build/lib/libSDL2_ttf.so)
set(SDL2_MIXER_LIBRARIES /root/workspace/build/lib/libSDL2_mixer.so)

include_directories(
	${CMAKE_SOURCE_DIR}/core/include
	${CMAKE_SOURCE_DIR}/core/include/sqlite
	${SDL2_INCLUDE_DIRS}
)

# Find all .cpp and .cc files in the source directory
file(GLOB_RECURSE SOURCES "*.cc")
# Add sqlite algamation file to source files
list(APPEND SOURCES "core/include/sqlite/sqlite3.c")

add_executable(retrodex ${SOURCES})

# link the libraries based on our above
target_link_libraries(retrodex 
	${SDL2_LIBRARIES} 
	${SDL2_IMAGE_LIBRARIES} 
	${SDL2_TTF_LIBRARIES} 
	${SDL2_MIXER_LIBRARIES} 
	${CMAKE_DL_LIBS} 
	dl
	pthread 
	stdc++fs # why do I need this even when specifying c++ standard 17
)

set_target_properties(retrodex PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

install(TARGETS retrodex DESTINATION bin)
