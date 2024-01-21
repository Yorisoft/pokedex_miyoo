cmake_minimum_required(VERSION 3.10)
project(pokedex)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)

set(SOURCE_DIR /root/workspace/pokedex)

#SDL
set(SDL2_INCLUDE_DIRS /root/workspace/build/include)
set(SDL2_LIBRARIES /root/workspace/build/lib/libSDL2.so)
set(SDL2_IMAGE_LIBRARIES /root/workspace/build/lib/libSDL2_image.so)
set(SDL2_TTF_LIBRARIES /root/workspace/build/lib/libSDL2_ttf.so)

#SQLITE
set(SQLITE_INCLUDE_DIRS ${SOURCE_DIR}/sqlite)
set(SQLITE_SOURCE ${SOURCE_DIR}/sqlite/sqlite3.c)

#CURL
set(CURL_LIBRARIES /root/workspace/curl-7.64.0/lib/.libs/libcurl.so)
set(CURL_INCLUDE_DIR /root/workspace/curl-7.64.0/include)

include_directories(${SDL2_INCLUDE_DIRS} ${SOURCE_DIR} ${SQLITE_INCLUDE_DIRS} ${CURL_INCLUDE_DIR})

add_executable(Pokedex ${SOURCE_DIR}/pokedex.cpp ${SQLITE_SOURCE})
add_executable(DownloadIcons ${SOURCE_DIR}/icons_downloader.cpp ${CURL_LIBRARIES} ${SQLITE_SOURCE})
add_executable(DownloadSprites ${SOURCE_DIR}/sprites_downloader.cpp ${CURL_LIBRARIES} ${SQLITE_SOURCE})
add_executable(DownloadAnimatedSprites ${SOURCE_DIR}/gifs_downloader.cpp ${CURL_LIBRARIES} ${SQLITE_SOURCE})

# link the libraries based on our above
target_link_libraries(Pokedex ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${CMAKE_DL_LIBS} -lpthread )
target_link_libraries(DownloadIcons ${CURL_LIBRARIES} ${CMAKE_DL_LIBS} -lpthread)
target_link_libraries(DownloadSprites ${CURL_LIBRARIES} ${CMAKE_DL_LIBS} -lpthread)
target_link_libraries(DownloadAnimatedSprites ${CURL_LIBRARIES} ${CMAKE_DL_LIBS} -lpthread)

install(TARGETS Pokedex DESTINATION /opt/miyoo/arm-buildroot-linux-gnueabihf/sysroot/build)

#install(TARGETS Pokedex DownloadIcons DownloadSprites DownloadAnimatedSprites DESTINATION /opt/miyoo/arm-buildroot-linux-gnueabihf/sysroot/build)