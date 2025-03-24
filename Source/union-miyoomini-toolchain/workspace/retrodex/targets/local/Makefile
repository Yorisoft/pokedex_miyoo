BUILD_DIR = build
BINARY_OUTPUT = bin
TARGET_NAME = retrodex

# Core directories
CORE_DIR = core
UTILS_DIR = $(CORE_DIR)/utils
ACTIVITIES_DIR = $(CORE_DIR)/activities
SQLITE_DIR = $(CORE_DIR)/include/sqlite

# Compiler flags
CXX = /usr/bin/c++
CXXFLAGS = -I$(CORE_DIR)/include -I$(UTILS_DIR) -I$(SQLITE_DIR) -I/usr/include/SDL2 -g -std=gnu++17
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Source groupings
ACTIVITIES_SOURCES = \
    $(ACTIVITIES_DIR)/PokedexActivity.cc \
    $(ACTIVITIES_DIR)/PokedexActivityEvent.cc \
    $(ACTIVITIES_DIR)/PokedexActivityManager.cc \
    $(ACTIVITIES_DIR)/setting/*.cc \
    $(ACTIVITIES_DIR)/intro/*.cc \
    $(ACTIVITIES_DIR)/menu/*.cc \
    $(ACTIVITIES_DIR)/list/*.cc \
    $(ACTIVITIES_DIR)/pokemons/info/*.cc \
    $(ACTIVITIES_DIR)/pokemons/stats/*.cc \
    $(ACTIVITIES_DIR)/pokemons/moves/*.cc \
    $(ACTIVITIES_DIR)/pokemons/location/*.cc \
    $(ACTIVITIES_DIR)/pokemons/evolution/*.cc 

UTILS_SOURCES = \
    $(UTILS_DIR)/*.cc \

SQLITE_SOURCE = $(SQLITE_DIR)/sqlite3.c

# Root-level source file
ROOT_SOURCE = Pokedex.cc

all: rebuild

rebuild: clean cmake_configure build

clean:
	rm -rfv $(BUILD_DIR)/*

cmake_configure: clean
	cd $(BUILD_DIR) && cmake ..

build: cmake_configure
	cd $(BUILD_DIR) && make

run:
	cp -v $(BUILD_DIR)/$(BINARY_OUTPUT)/$(TARGET_NAME) . && \
    ./$(TARGET_NAME)

# Explicit rule for Pokedex.cc
Pokedex:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ROOT_SOURCE).o -c $(ROOT_SOURCE)

# Individual targets for activities
PokedexActivity:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/PokedexActivity.cc.o -c $(ACTIVITIES_DIR)/PokedexActivity.cc

PokedexActivityEvent:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/PokedexActivityEvent.cc.o -c $(ACTIVITIES_DIR)/PokedexActivityEvent.cc

PokedexActivityManager:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/PokedexActivityManager.cc.o -c $(ACTIVITIES_DIR)/PokedexActivityManager.cc

PokedexActivityIntro:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/intro/PokedexActivityIntro.cc.o -c $(ACTIVITIES_DIR)/intro/PokedexActivityIntro.cc

PokedexActivityIntro-input:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/intro/input.cc.o -c $(ACTIVITIES_DIR)/intro/input.cc

PokedexActivityIntro-render:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/intro/render.cc.o -c $(ACTIVITIES_DIR)/intro/render.cc

PokedexActivityList:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/list/PokedexActivityList.cc.o -c $(ACTIVITIES_DIR)/list/PokedexActivityList.cc

PokedexActivityMenu:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/menu/PokedexActivityMenu.cc.o -c $(ACTIVITIES_DIR)/menu/PokedexActivityMenu.cc && \
	g++ -Wall $(BUILD_DIR)/CMakeFiles/retrodex.dir/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*/*/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(SQLITE_DIR)/*.o \
    -o $(BUILD_DIR)/$(BINARY_OUTPUT)/$(TARGET_NAME) $(LDFLAGS)
                                                                                 

PokedexActivityMenu-input:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/menu/input.cc.o -c $(ACTIVITIES_DIR)/menu/input.cc && \
	g++ -Wall $(BUILD_DIR)/CMakeFiles/retrodex.dir/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*/*/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(SQLITE_DIR)/*.o \
    -o $(BUILD_DIR)/$(BINARY_OUTPUT)/$(TARGET_NAME) $(LDFLAGS)

PokedexActivityMenu-render:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/menu/render.cc.o -c $(ACTIVITIES_DIR)/menu/render.cc && \
	g++ -Wall $(BUILD_DIR)/CMakeFiles/retrodex.dir/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*/*/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(SQLITE_DIR)/*.o \
    -o $(BUILD_DIR)/$(BINARY_OUTPUT)/$(TARGET_NAME) $(LDFLAGS)

PokedexActivity_PokemonView_Evolution:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/pokemons/evolution/PokedexActivity_PokemonView_Evolution.cc.o -c $(ACTIVITIES_DIR)/pokemon_evolution/PokedexActivity_PokemonView_Evolution.cc
                                                                                 
PokedexActivity_PokemonView_Info:                                                
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/pokemons/info/PokedexActivity_PokemonView_Info.cc.o -c $(ACTIVITIES_DIR)/pokemon_info/PokedexActivity_PokemonView_Info.cc
                                                                                 
PokedexActivity_PokemonView_Location:                                            
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/pokemons/location/PokedexActivity_PokemonView_Location.cc.o -c $(ACTIVITIES_DIR)/pokemon_location/PokedexActivity_PokemonView_Location.cc
                                                                                 
PokedexActivity_PokemonView_Moves:                                               
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/pokemons/moves/PokedexActivity_PokemonView_Moves.cc.o -c $(ACTIVITIES_DIR)/pokemons/moves/PokedexActivity_PokemonView_Moves.cc
                                                                                 
PokedexActivity_PokemonView_Moves-input:                                               
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/pokemons/moves/input.cc.o -c $(ACTIVITIES_DIR)/pokemons/moves/input.cc
                                                                                 
PokedexActivity_PokemonView_Moves-render:                                               
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/pokemons/moves/render.cc.o -c $(ACTIVITIES_DIR)/pokemons/moves/render.cc
                                                                                 
PokedexActivity_PokemonView_Stats:                                               
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/pokemons/stats/PokedexActivity_PokemonView_Stats.cc.o -c $(ACTIVITIES_DIR)/pokemon_stats/PokedexActivity_PokemonView_Stats.cc

PokedexActivitySetting:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/setting/PokedexActivitySetting.cc.o -c $(ACTIVITIES_DIR)/setting/PokedexActivitySetting.cc

# Individual targets for utils
AssetManager:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/AssetManager.cc.o -c $(UTILS_DIR)/AssetManager.cc

PokeSurface:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/PokeSurface.cc.o -c $(UTILS_DIR)/PokeSurface.cc

PokedexAssets:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/PokedexAssets.cc.o -c $(UTILS_DIR)/PokedexAssets.cc

PokedexDB:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/PokedexDB.cc.o -c $(UTILS_DIR)/PokedexDB.cc

Pokemon:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/Pokemon.cc.o -c $(UTILS_DIR)/Pokemon.cc

SQLstatements:
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/SQLstatements.cc.o -c $(UTILS_DIR)/SQLstatements.cc

# Special case for SQLite
sqlite3:
	/usr/bin/cc -I$(CORE_DIR)/include -I$(UTILS_DIR) -I$(SQLITE_DIR) -I/usr/include/SDL2 -g \
    -o $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(SQLITE_SOURCE).o -c $(SQLITE_SOURCE)

CUSTOM_COMMAND:
	g++ -Wall $(BUILD_DIR)/CMakeFiles/retrodex.dir/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(ACTIVITIES_DIR)/*/*/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(UTILS_DIR)/*.o \
    $(BUILD_DIR)/CMakeFiles/retrodex.dir/$(SQLITE_DIR)/*.o \
    -o $(BUILD_DIR)/$(BINARY_OUTPUT)/$(TARGET_NAME) $(LDFLAGS)

# Linking rule
$(TARGET_NAME): CUSTOM_COMMAND

.PHONY: all rebuild clean cmake_configure build run sqlite3 Pokedex \
		PokedexActivity PokedexActivityEvent PokedexActivityManager PokedexActivityIntro \
        PokedexActivityList PokedexActivityMenu PokedexActivity_PokemonView_Evolution \
        PokedexActivity_PokemonView_Info PokedexActivity_PokemonView_Location \
        PokedexActivity_PokemonView_Moves PokedexActivity_PokemonView_Stats PokedexActivitySetting \
        AssetManager PokeSurface PokedexAssets PokedexDB Pokemon SQLstatements
