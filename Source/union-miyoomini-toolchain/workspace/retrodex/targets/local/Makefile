BUILD_DIR = build
BINARY_OUTPUT = bin
TARGET_NAME = retrodex

rebuild: clean cmake_configure build

clean:
	rm -rfv $(BUILD_DIR)/*  # Or just rm -rf build if the build dir is dedicated

cmake_configure: clean
	cd $(BUILD_DIR) && cmake ..

build: cmake_configure
	ls && \
	cd $(BUILD_DIR) && make

run:
	cp -v $(BUILD_DIR)/$(BINARY_OUTPUT)/$(TARGET_NAME) . && \
	./$(TARGET_NAME)

# Individual Files
# Requires running build once
PokedexActivitySetting:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o CMakeFiles/retrodex.dir/core/PokedexActivitySetting.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivitySetting.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivityMenu:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivityMenu.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivityMenu.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivityList: 
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivityList.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivityList.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivity_PokemonView_Moves:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivity_PokemonView_Moves.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivity_PokemonView_Moves.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivity_PokemonView_Location:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivity_PokemonView_Location.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivity_PokemonView_Location.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
