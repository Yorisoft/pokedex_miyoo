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
Pokedex.cc:
	/usr/bin/c++ -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/Pokedex.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/Pokedex.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

AssetManager:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/AssetManager.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/AssetManager.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokeSurface:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokeSurface.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokeSurface.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivity:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivity.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivity.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivityEvent:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivityEvent.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivityEvent.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivityIntro:
	/usr/bin/c++ -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivityIntro.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivityIntro.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivityList:
	/usr/bin/c++ -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivityList.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivityList.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivityManager:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivityManager.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivityManager.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivityMenu:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivityMenu.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivityMenu.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivitySetting:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivitySetting.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivitySetting.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivity_PokemonView_Evolution:
	/usr/bin/c++ -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivity_PokemonView_Evolution.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivity_PokemonView_Evolution.cc && \
	g++ -Wall build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivity_PokemonView_Info:
	/usr/bin/c++ -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivity_PokemonView_Info.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivity_PokemonView_Info.cc && \
	g++ -Wall build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivity_PokemonView_Location:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivity_PokemonView_Location.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivity_PokemonView_Location.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivity_PokemonView_Moves:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivity_PokemonView_Moves.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivity_PokemonView_Moves.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexActivity_PokemonView_Stats:
	/usr/bin/c++ -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexActivity_PokemonView_Stats.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexActivity_PokemonView_Stats.cc && \
	g++ -Wall build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexAssets:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexAssets.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexAssets.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

PokedexDB:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/PokedexDB.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/PokedexDB.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

Pokemon:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/Pokemon.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/Pokemon.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SQLstatements:
	/usr/bin/c++  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -std=gnu++17 -o build/CMakeFiles/retrodex.dir/core/SQLstatements.cc.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/SQLstatements.cc && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

sqlite3:
	/usr/bin/cc  -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/utils -I/home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite -I/usr/include/SDL2 -g -o build/CMakeFiles/retrodex.dir/core/include/sqlite/sqlite3.c.o -c /home/yorisoft/code/my-gh/pokedex_miyoo/Source/union-miyoomini-toolchain/workspace/retrodex/core/include/sqlite/sqlite3.c && \
	g++ -Wall  build/CMakeFiles/retrodex.dir/*.o build/CMakeFiles/retrodex.dir/core/*.o build/CMakeFiles/retrodex.dir/core/include/sqlite/*.o  -o build/bin/retrodex -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
