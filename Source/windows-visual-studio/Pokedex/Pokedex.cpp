// Window Settings
#define WINDOW_HEIGHT 480                   // window height in pixels
#define WINDOW_WIDTH 640                    // window width in pixels
#define DEPTH 16                            // window depth in pixels
#define MAX_VISIBLE_ITEMS 5                 // Set the maximum number of visible

// Controller inputs
#define SW_BTN_UP SDLK_UP
#define SW_BTN_DOWN SDLK_DOWN
#define SW_BTN_LEFT SDLK_LEFT
#define SW_BTN_RIGHT SDLK_RIGHT

#define SW_BTN_A SDLK_SPACE
#define SW_BTN_B SDLK_LCTRL
#define SW_BTN_X SDLK_LSHIFT
#define SW_BTN_Y SDLK_LALT

#define SW_BTN_L1 SDLK_e
#define SW_BTN_R1 SDLK_t
#define SW_BTN_L2 SDLK_TAB
#define SW_BTN_R2 SDLK_BACKSPACE

#define SW_BTN_SELECT SDLK_RCTRL
#define SW_BTN_START SDLK_RETURN
#define SW_BTN_MENU SDLK_ESCAPE
#define SW_BTN_POWER SDLK_FIRST

#include "sqlite/sqlite3.h"					// sqlite library
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream> 
#include <map>
#include <SDL.h>                       		
#include <SDL_image.h>                   	
#include <SDL_ttf.h>
//#include <SDL_mixer.h>
using namespace std;

enum class View {
    LIST,
    STATS
};

// Function to initialize SDL components
void InitializeSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, SDL_Surface** screen, TTF_Font** font) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    *window = SDL_CreateWindow(
        "Pokedex",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    *texture = SDL_CreateTexture(
        *renderer,
        SDL_PIXELFORMAT_RGB565,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    *screen = SDL_CreateRGBSurface(
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        DEPTH,
        0, 0, 0, 0
    );

    if (TTF_Init() == -1) {
        cout << "SDL could not initialize TTF_Init: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    *font = TTF_OpenFont("res/font/Pokemon_GB.ttf", 20);
    if (*font == NULL) {
        cout << "TTF_OpenFont: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_FillRect(
        *screen,
        &(*screen)->clip_rect,
        SDL_MapRGB((*screen)->format, 0x00, 0x00, 0x00)
    );

    /*if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }*/
}

// Function to clean up SDL components
void CleanupSDL(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface* screen) {
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    //Mix_Quit();
}

// Function to open the SQLite database
sqlite3* OpenDatabase(const string& dbName) {
    sqlite3* db;
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc != SQLITE_OK) {
        cout << "SQLite could not open file! SQLite Error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    return db;
}

//  copy and pasted from the internet. Not super sure what the heck its doing. 
static int callback(void* results, int argc, char** argv, char** azColName) {
    vector<string> row;
    static_cast<vector<vector<string>>*>(results)->emplace_back(argv, argv + argc);
    return 0;
};

// Function to execute a SQL query and return the result as a string
vector<vector<string>> executeSQL(const string& sql) {
    vector<vector<string>> results;
    sqlite3* db;
    int rc;
    char* zErrMsg = 0;
    rc = sqlite3_open("res/db/pokedex.sqlite", &db);

    if (rc != SQLITE_OK) {
        cout << "SQLite coult not open file! SQLite Error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return results;
    }
    else {
        if (sqlite3_exec(db, sql.c_str(), callback, &results, &zErrMsg) != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        sqlite3_close(db);
        return results;
    }
};

// Function to close the SQLite database
void CloseDatabase(sqlite3* db) {
    sqlite3_close(db);
}

void HandleInputs(SDL_Event& windowEvent, bool& quit, View& currentView, int& selectedItem, int& topItem, int& currentGenIndex, vector<int>& nextGenID, bool& needsRendering) {
    while (SDL_PollEvent(&windowEvent))
    {
        if (windowEvent.type == SDL_QUIT) {
            quit = true;
        }
        else if (windowEvent.type == SDL_KEYDOWN) {
            switch (windowEvent.key.keysym.sym) {
            case SW_BTN_A:
                if (currentView == View::LIST) currentView = View::STATS;
                needsRendering = true;
                break;
            case SW_BTN_B:
                if (currentView == View::STATS) currentView = View::LIST;
                needsRendering = true;
                break;
            case SW_BTN_RIGHT:
                //currentView = View::STATS;
                needsRendering = true;
                break;
            case SW_BTN_LEFT:
                //currentView = View::LIST;
                needsRendering = true;
                break;
            case SW_BTN_UP:
                needsRendering = true;
                if (selectedItem > 0) selectedItem--;
                if (selectedItem < topItem) topItem = selectedItem;
                break;
            case SW_BTN_DOWN:
                needsRendering = true;
                if (selectedItem < 649 - 1) selectedItem++;
                if (selectedItem >= topItem) topItem++;
                break;
            case SW_BTN_R1:
                needsRendering = true;
                if (selectedItem < 649 - 10) selectedItem += 10;
                if (selectedItem >= topItem) topItem += 10;
                break;
            case SW_BTN_L1:
                needsRendering = true;
                if (selectedItem > 9) selectedItem -= 10;
                if (selectedItem < topItem) topItem -= 10;
                break;
            case SW_BTN_R2:
                needsRendering = true;
                if (currentGenIndex < nextGenID.size() - 1) {
                    currentGenIndex++;
                    selectedItem = nextGenID[currentGenIndex];
                }
                break;
            case SW_BTN_L2:
                needsRendering = true;
                if (currentGenIndex > 0) {
                    currentGenIndex--;
                    selectedItem = nextGenID[currentGenIndex];
                }
                break;
            case SDLK_ESCAPE:
                quit = true;
                break;
            default:
                break;
            }
        }
    }
}

// Function to load an image
SDL_Surface* LoadImage(SDL_Surface** imgSurface, string& imagePath) {
    cout << "starting: LoadImage" << endl;
    SDL_Surface* optimizedSurface = NULL;
    *imgSurface = IMG_Load(imagePath.c_str());
    if (*imgSurface == NULL) {
        cout << "Unable to load image! SDL Error: " << IMG_GetError() << endl;
        exit(EXIT_FAILURE);
    }
    optimizedSurface = SDL_ConvertSurfaceFormat(*imgSurface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(*imgSurface);
    if (optimizedSurface == NULL) {
        cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << endl;
        exit;
    }
    cout << "ending: LoadImage" << endl;
    return optimizedSurface;
}

// Function to render text
TTF_Font* OpenFont(TTF_Font** font, const string& fontName, int size) {
    *font = TTF_OpenFont(fontName.c_str(), size);
    if (font == NULL) {
        cout << "TTF_OpenFont: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }
    return *font;
}

// Function to render text
//SDL_Surface* RenderText(SDL_Surface* textSurface, const string& text, TTF_Font* font, const SDL_Color& color) {
//    SDL_Surface*  textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
//    if (textSurface == NULL) {
//        cout << "Unable to render text! SDL Error: " << TTF_GetError() << endl;
//        exit(EXIT_FAILURE);
//    }
//    return textSurface;
//}

// Function to get Pokemon data from the database
vector<vector<string>> GetPokemonData(const string& sqlQuery) {
    return executeSQL(sqlQuery);
}

// Function to get Pokemon stats from the data
//vector<string> GetPokemonStats(const vector<string>& pokemon) {
//    // Extract and process Pokemon stats from the data
//    // ... (your existing code for processing stats)
//    //return processedStats;
//}

// Function to render minimal Pokemon stats
int RenderPokemonMinimalLabel(SDL_Surface** screen, SDL_Surface** pokeListLabelSurface, TTF_Font** font) {
    //cout << "starting: RenderPokemonMinimalStats" << endl;
    SDL_Color color = { 255, 255, 255 };
    SDL_Color highlightColor = { 255, 0, 0 };

    // Create a string stream to build the stats text
    stringstream statsStream;

    //statsStream << left << setw(5) << pokemonName << " ";
    statsStream << left << setw(12) << "ID";
    statsStream << left << setw(13) << "Name";
    statsStream << left << setw(4) << "Type";
    statsStream << right << setw(4) << "HP";
    statsStream << right << setw(4) << "Atk";
    statsStream << right << setw(4) << "Def";
    statsStream << right << setw(4) << "SpA";
    statsStream << right << setw(4) << "SpD";
    statsStream << right << setw(4) << "Spe";

    // Convert the stream to a string
    string statsText = statsStream.str();

    // Render the text into an SDL_Surface
    *pokeListLabelSurface = TTF_RenderText_Blended(
        *font,
        statsText.c_str(),
        color
    );
    if (*pokeListLabelSurface == NULL) {
        cout << "Unable to render text! SDL Error: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeListLabelRect;
    pokeListLabelRect.x = 0;                 // Align to the right
    pokeListLabelRect.y = 0;                 // Center vertically
    pokeListLabelRect.w = (*pokeListLabelSurface)->w;
    pokeListLabelRect.h = (*pokeListLabelSurface)->h;

    // save height
    int surfaceHeight = ((*pokeListLabelSurface)->h);

    // Clean up pokemonArtRect pokeListArtSurface
    SDL_BlitSurface(*pokeListLabelSurface, NULL, *screen, &pokeListLabelRect);
    SDL_FreeSurface(*pokeListLabelSurface);
    //SDL_FreeSurface(*pokeListLabelSurface);
    //cout << "starting: RenderPokemonMinimalStats" << endl;

    return surfaceHeight;
}

void setMaxListIDWidth(int& maxListIDWidth, string& id, TTF_Font** font) {
    // Set value for maxListIDWidth
    SDL_Surface* maxListIDSurfaceWidth = NULL;
    SDL_Color color = { 255, 255, 255 };

    maxListIDSurfaceWidth = TTF_RenderText_Blended(
        *font,
        id.c_str(),
        color
    );
    if (maxListIDSurfaceWidth == NULL) {
        cout << "Unable to render text! SDL Error: maxListIDSurfaceWidth " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    };
    maxListIDWidth = maxListIDSurfaceWidth->w;
}

// Function to render Pokemon ID
void RenderPokemonID(SDL_Surface** pokemonListEntrySurface, TTF_Font** font, vector<string>& pokemon, int& counter) {
    SDL_Surface* pokeListIDSurface = NULL, * maxListIDSurfaceWidth = NULL;
    SDL_Color color = { 255, 255, 255 };
    SDL_Color highlightColor = { 255, 0, 0 };

    pokeListIDSurface = TTF_RenderText_Blended(
        *font,
        pokemon[0].c_str(),
        counter == 0 ? highlightColor : color
    );
    if (pokeListIDSurface == NULL) {
        cout << "Unable to render text! SDL Error: pokeListIDSurface " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeListIDRect;
    pokeListIDRect.x = 0;
    pokeListIDRect.y = (*pokemonListEntrySurface)->h / 2 - pokeListIDSurface->h / 2;
    pokeListIDRect.w = (*pokemonListEntrySurface)->w;
    pokeListIDRect.h = (*pokemonListEntrySurface)->h;

    SDL_BlitSurface(pokeListIDSurface, NULL, *pokemonListEntrySurface, &pokeListIDRect);
    SDL_FreeSurface(pokeListIDSurface);
}

// Function to render Pokemon pixel art
void RenderPokemonListPixelArt(SDL_Surface** pokemonListEntrySurface, TTF_Font** font, vector<string>& pokemon, int& counter, int& maxListIDWidth) {
    cout << "starting: RenderPokemonPixelArt" << endl;
    string imagePath, typePath1, typePath2;
    SDL_Surface* optimizedSurface = NULL, * pokeListArtSurface = NULL,
        * pokeListType1Surface = NULL, * pokeListType2Surface = NULL, * pokeListNameSurface = NULL;

    // Load & Blits pokeListArtSurface
    // pixel art path
    imagePath = "res/icons/" + pokemon[1] + ".png";
    pokeListArtSurface = LoadImage(&pokeListArtSurface, imagePath);

    SDL_Rect pokemonArtRect;
    pokemonArtRect.x = maxListIDWidth;
    pokemonArtRect.y = ((*pokemonListEntrySurface)->h / 2) - (pokeListArtSurface->h / 2) - (pokeListArtSurface->h / 2);
    pokemonArtRect.w = pokeListArtSurface->w * 2;
    pokemonArtRect.h = pokeListArtSurface->h * 2;

    SDL_BlitScaled(pokeListArtSurface, NULL, *pokemonListEntrySurface, &pokemonArtRect);
    SDL_FreeSurface(pokeListArtSurface);
    cout << "after:SDL_BlitScaled - pokeListArtSurface" << endl;
    // Print pokemon name
    // 
    SDL_Color color = { 255, 255, 255 };
    SDL_Color highlightColor = { 255, 0, 0 };

    string pokemonName = pokemon[1];
    pokemonName[0] = toupper(pokemonName[0]);
    // If an underscore is found, get the substring up to the underscore
    size_t underscorePos = pokemonName.find('-');
    pokemonName = underscorePos != string::npos ? pokemonName.substr(0, underscorePos) : pokemonName;

    pokeListNameSurface = TTF_RenderText_Blended(
        *font,
        pokemonName.c_str(),
        counter == 0 ? highlightColor : color
    );
    cout << "after:TTF_RenderText_Blended - pokeListNameSurface" << endl;
    if (pokeListNameSurface == NULL) {
        cout << "Unable to render text! SDL Error: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    };

    SDL_Rect pokeListNameRect;
    pokeListNameRect.x = pokemonArtRect.x + pokemonArtRect.w - 5;
    pokeListNameRect.y = (*pokemonListEntrySurface)->h / 2 - pokeListNameSurface->h / 2;             // Center vertically
    pokeListNameRect.w = pokeListNameSurface->w;
    pokeListNameRect.h = pokeListNameSurface->h;
    SDL_BlitScaled(pokeListNameSurface, NULL, *pokemonListEntrySurface, &pokeListNameRect);
    SDL_FreeSurface(pokeListNameSurface);
    cout << "after:SDL_BlitScaled - pokeListNameSurface" << endl;
    // Load & Blits pokeListType1Surface
    // pixel art path
    typePath1 = "res/types/" + pokemon[8] + ".png";
    pokeListType1Surface = LoadImage(&pokeListType1Surface, typePath1);

    SDL_Rect pokemonType1Rect;
    pokemonType1Rect.x = ((*pokemonListEntrySurface)->w / 2) - 10;
    pokemonType1Rect.y = ((*pokemonListEntrySurface)->h - pokeListType1Surface->h * 2) / 2;
    pokemonType1Rect.w = pokeListType1Surface->w * 1.1;
    pokemonType1Rect.h = pokeListType1Surface->h * 1.1;

    SDL_BlitScaled(pokeListType1Surface, NULL, *pokemonListEntrySurface, &pokemonType1Rect);
    SDL_FreeSurface(pokeListType1Surface);
    cout << "after:SDL_BlitScaled - pokeListType1Surface" << endl;
    if (!pokemon[9].empty()) {
        // Load & Blits pokeListType1Surface
        // pixel art path
        typePath2 = "res/types/" + pokemon[9] + ".png";
        pokeListType2Surface = LoadImage(&pokeListType2Surface, typePath2);

        SDL_Rect pokemonType2Rect;
        pokemonType2Rect.x = ((*pokemonListEntrySurface)->w / 2) - 10;
        pokemonType2Rect.y = pokemonType1Rect.y + pokemonType1Rect.h;
        pokemonType2Rect.w = pokeListType2Surface->w * 1.1;
        pokemonType2Rect.h = pokeListType2Surface->h * 1.1;

        SDL_BlitScaled(pokeListType2Surface, NULL, *pokemonListEntrySurface, &pokemonType2Rect);
        SDL_FreeSurface(pokeListType2Surface);
        cout << "after:SDL_BlitScaled - pokeListType2Surface" << endl;
    }
    cout << "starting: RenderPokemonPixelArt" << endl;
}

// Function to render minimal Pokemon stats
void RenderPokemonMinimalStats(SDL_Surface** pokemonListEntrySurface, TTF_Font** font, vector<string>& pokemon, int& counter) {
    //cout << "starting: RenderPokemonMinimalStats" << endl;
    SDL_Color color = { 255, 255, 255 };
    SDL_Color highlightColor = { 255, 0, 0 };

    // Create a string stream to build the stats text
    stringstream statsStream;

    statsStream << right << setw(4) << pokemon[2];  //  hp
    statsStream << right << setw(4) << pokemon[3];  //  attack
    statsStream << right << setw(4) << pokemon[4];  //  defense
    statsStream << right << setw(4) << pokemon[5];  //  special - attack
    statsStream << right << setw(4) << pokemon[6];  //  special - defense
    statsStream << right << setw(4) << pokemon[7];  //  speed special 

    // Convert the stream to a string
    string statsText = statsStream.str();

    // Render the text into an SDL_Surface
    SDL_Surface* pokeListStatsSurface = NULL;

    pokeListStatsSurface = TTF_RenderText_Blended(
        *font,
        statsText.c_str(),
        counter == 0 ? highlightColor : color
    );
    if (pokeListStatsSurface == NULL) {
        cout << "Unable to render text! SDL Error: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    SDL_Rect pokemonStatsRect;
    pokemonStatsRect.x = ((*pokemonListEntrySurface)->w - pokeListStatsSurface->w);                 // Align to the right
    pokemonStatsRect.y = (*pokemonListEntrySurface)->h / 2 - pokeListStatsSurface->h / 2;             // Center vertically
    pokemonStatsRect.w = pokeListStatsSurface->w;
    pokemonStatsRect.h = pokeListStatsSurface->h;
    // Clean up pokemonArtRect pokeListArtSurface
    SDL_BlitSurface(pokeListStatsSurface, NULL, *pokemonListEntrySurface, &pokemonStatsRect);
    SDL_FreeSurface(pokeListStatsSurface);

    //cout << "starting: RenderPokemonMinimalStats" << endl;
}

// Function to render the entire Pokedex list
void RenderPokedexList(SDL_Surface** screen, SDL_Renderer** renderer, TTF_Font** font, vector<vector<string>>& results, int& topItem, int& selectedItem, int& maxListIDWidth, int& maxListLabelHight, bool& needsRendering) {
    if (needsRendering) {
        cout << "Next loop" << endl;
        cout << "selectedItem: " + to_string(selectedItem) << endl << endl;
        cout << "topItem: " + to_string(topItem) << endl << endl;
        // Get all * from pokemon Table
        // HERE YOU GET ALL THE DATA YOU NEED. 

        TTF_Font* listFont = NULL;
        SDL_Surface* pokeListLabelSurface = NULL,
            * pokemonListEntrySurface = NULL,
            * textListSurface = NULL,
            * pokeListIDSurface = NULL;
        ostringstream textStream;
        SDL_Color color = { 255, 255, 255 };
        SDL_Color highlightColor = { 255, 0, 0 };
        string fontPath = "res/font/Pokemon_GB.ttf";
        int pokemonListSurfaceHeight = static_cast<int>(WINDOW_HEIGHT / 5);

        // Define the map
        map<string, vector<vector<int>>> color_map;

        // Insert the key-value pair
        color_map["bug"] = { {103,116,69}, {81,91,52} };                    // Ebony
        color_map["dark"] = { {55,55,65}, {40,40,50} };                     // DimGray and DarkGray
        color_map["dragon"] = { {76,63,105}, {59,50,81} };                  // SteelBlue and CornflowerBlue
        color_map["electric"] = { {90, 90, 30}, {70, 70, 20} };             // Field drab, Dark drab brown
        color_map["fairy"] = { {121,67,103}, {100,48,74} };                 // Pink and LightPink
        color_map["fighting"] = { {78,45,45}, {57,34,34} };                 // Orange and Gold
        color_map["fire"] = { {111, 53, 17}, {86, 42, 19} };                // Russet, Seal brown
        color_map["flying"] = { {76,91,87}, {54,71,68} };                   // Feldgrau, Dark slate grey
        color_map["ghost"] = { {85,70,93}, {66,51,74} };                    // SlateBlue and MediumSlateBlue
        color_map["grass"] = { {44, 61, 43}, {40, 52, 39} };                // Dark green, Black Olive
        color_map["ground"] = { {89,69,52}, {71,59,46} };                   // Walnut brown, Taupe
        color_map["ice"] = { {73,84,91}, {55,60,64} };                      // Onyx, Gunmetal
        color_map["normal"] = { {105,112,95}, {76,83,66} };                 // BlanchedAlmond and Cornsilk   {106,102,83}, {94,92,79}
        color_map["poison"] = { {92,77,89}, {67,54,71} };                   // MediumPurple and Purple
        color_map["psychic"] = { {91,63,79}, {76,45,63} };                  // Plum and Violet
        color_map["rock"] = { {71,67,63}, {61,58,55} };
        color_map["steel"] = { {99,117,125}, {79,94,100} };                 // Silver and LightGray
        color_map["water"] = { {43, 58, 78}, {33, 46, 62} };                // Charcoal, Gunmetal


        listFont = OpenFont(&listFont, fontPath, 12);
        maxListLabelHight = RenderPokemonMinimalLabel(screen, &pokeListLabelSurface, &listFont);
        TTF_CloseFont(listFont);


        string maxIdNum = "000";
        listFont = OpenFont(&listFont, fontPath, 12);
        setMaxListIDWidth(maxListIDWidth, maxIdNum, &listFont);
        TTF_CloseFont(listFont);

        // Iterate over all Pokémon in the results
        for (int i = 0; i < MAX_VISIBLE_ITEMS && selectedItem + i < results.size(); i++) {
            cout << "starting.." << i << endl;
            cout << "selectedItem: " << selectedItem << endl;
            cout << "topItem: " << topItem << endl;

            // Set the background color based on pokemon type. 
            vector<string> pokemon = results[selectedItem + i];
            string pokemonType = pokemon[8];
            int pokemonID = stoi(pokemon[0]);
            int colorIndex = (pokemonID - 1) % 2; // This will alternate between 0 and 1            
            SDL_Color rainbowColors = {
                static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][0]),
                static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][1]),
                static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][2])
            };

            pokemonListEntrySurface = SDL_CreateRGBSurface(
                0,
                WINDOW_WIDTH,
                pokemonListSurfaceHeight,
                DEPTH,
                0, 0, 0, 0
            );
            SDL_FillRect(
                pokemonListEntrySurface,
                NULL,
                SDL_MapRGBA(
                    pokemonListEntrySurface->format,
                    rainbowColors.r,
                    rainbowColors.g,
                    rainbowColors.b,
                    128
                )
            );

            int labelHeight = pokemonListEntrySurface->h;
            SDL_Rect pokeListEntry;
            pokeListEntry.x = 0;
            pokeListEntry.y = maxListLabelHight + (i * pokemonListSurfaceHeight);
            pokeListEntry.w = pokemonListEntrySurface->w;
            pokeListEntry.h = pokemonListEntrySurface->h;

            // TODO: Make get pokemonLabel()
            bool isSelected = (i == selectedItem);
            listFont = OpenFont(&listFont, fontPath, 15);
            // TODO: Make get pokemonID()
            RenderPokemonID(&pokemonListEntrySurface, &listFont, pokemon, i);
            TTF_CloseFont(listFont);

            cout << "after RenderPokemonID" << endl;
            cout << "i" << i << endl;
            cout << "selectedItem: " << selectedItem << endl;
            cout << "topItem: " << topItem << endl;

            // TODO: Make get pokemonPixelArt()
            listFont = OpenFont(&listFont, fontPath, 15);
            RenderPokemonListPixelArt(&pokemonListEntrySurface, &listFont, pokemon, i, maxListIDWidth);
            TTF_CloseFont(listFont);

            cout << "after RenderPokemonPixelArt" << endl;
            cout << "i" << i << endl;
            cout << "selectedItem: " << selectedItem << endl;
            cout << "topItem: " << topItem << endl;

            // TODO: Make get pokemonMinimalStats()
            listFont = OpenFont(&listFont, fontPath, 12);
            RenderPokemonMinimalStats(&pokemonListEntrySurface, &listFont, pokemon, i);
            TTF_CloseFont(listFont);

            cout << "after RenderPokemonMinimalStats" << endl;
            cout << "i" << i << endl;
            cout << "selectedItem: " << selectedItem << endl;
            cout << "topItem: " << topItem << endl;

            SDL_BlitSurface(pokemonListEntrySurface, NULL, *screen, &pokeListEntry);
            SDL_FreeSurface(pokemonListEntrySurface);

        };
        needsRendering = false;
    };
};

vector<string> splitLines(const string& text) {
    vector<string> lines;
    stringstream ss(text);
    string line;
    while (getline(ss, line, '\n')) {
        lines.push_back(line);
    }
    return lines;
}

// Function to render the entire Pokedex list
void RenderPokedexStats(SDL_Surface** screen, SDL_Renderer** renderer, TTF_Font** font, vector<vector<string>>& results, vector<vector<string>>& encounterData, int& topItem, int& selectedItem, int& maxListIDWidth, int& maxListLabelHight, bool& needsRendering) {
    if (needsRendering) {
        cout << "starting: RenderPokedexStats" << endl;
        string imagePath, typePath1, typePath2;
        SDL_Surface* optimizedSurface = NULL, * pokeStatArtSurface = NULL,
            * pokeStatsType1Surface = NULL, * pokeStatsType2Surface = NULL, * pokeListNameSurface = NULL;

        SDL_Surface* pokeStatsSurface = NULL;
        SDL_Color color = { 255, 255, 255 };
        SDL_Color highlightColor = { 255, 0, 0 };
        string fontPath = "res/font/Pokemon_GB.ttf";
        TTF_Font* statsFont = NULL;
        // Get the selected Pokémon's stats from the results vector
        vector<string> pokemon = results[selectedItem];
        string pokemonNme = pokemon[1];

        // Define the map
        map<string, vector<vector<int>>> color_map;

        // Insert the key-value pair
        color_map["bug"] = { {103,116,69}, {81,91,52} };                    // Ebony
        color_map["dark"] = { {55,55,65}, {40,40,50} };           // DimGray and DarkGray
        color_map["dragon"] = { {76,63,105}, {59,50,81} };                  // SteelBlue and CornflowerBlue
        color_map["electric"] = { {90, 90, 30}, {70, 70, 20} };                  // Field drab, Dark drab brown
        color_map["fairy"] = { {121,67,103}, {100,48,74} };          // Pink and LightPink
        color_map["fighting"] = { {78,45,45}, {57,34,34} };           // Orange and Gold
        color_map["fire"] = { {111, 53, 17}, {86, 42, 19} };                // Russet, Seal brown
        color_map["flying"] = { {76,91,87}, {54,71,68} };                   // Feldgrau, Dark slate grey
        color_map["ghost"] = { {85,70,93}, {66,51,74} };           // SlateBlue and MediumSlateBlue
        color_map["grass"] = { {44, 61, 43}, {40, 52, 39} };                // Dark green, Black Olive
        color_map["ground"] = { {89,69,52}, {71,59,46} };                     // Walnut brown, Taupe
        color_map["ice"] = { {73,84,91}, {55,60,64} };                      // Onyx, Gunmetal
        color_map["normal"] = { {105,112,95}, {76,83,66} };         // BlanchedAlmond and Cornsilk   {106,102,83}, {94,92,79}
        color_map["poison"] = { {92,77,89}, {67,54,71} };                   // MediumPurple and Purple
        color_map["psychic"] = { {91,63,79}, {76,45,63} };        // Plum and Violet
        color_map["rock"] = { {71,67,63}, {61,58,55} };
        color_map["steel"] = { {99,117,125}, {79,94,100} };          // Silver and LightGray
        color_map["water"] = { {43, 58, 78}, {33, 46, 62} };                // Charcoal, Gunmetal

        //---------------------------------------------------------

        string pokemonType = pokemon[8];
        int pokemonID = stoi(pokemon[0]);
        int colorIndex = (pokemonID - 1) % 2; // This will alternate between 0 and 1            
        SDL_Color rainbowColors = {
            static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][0]),
            static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][1]),
            static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][2])
        };

        pokeStatsSurface = SDL_CreateRGBSurface(
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            DEPTH,
            0, 0, 0, 0
        );
        SDL_FillRect(
            pokeStatsSurface,
            NULL,
            SDL_MapRGBA(
                pokeStatsSurface->format,
                rainbowColors.r,
                rainbowColors.g,
                rainbowColors.b,
                128
            )
        );

        int labelHeight = pokeStatsSurface->h;
        SDL_Rect pokeStatsRect;
        pokeStatsRect.x = 0;
        pokeStatsRect.y = 0;
        pokeStatsRect.w = pokeStatsSurface->w;
        pokeStatsRect.h = pokeStatsSurface->h;

        // Load & Blits pokeListArtSurface
        // pixel art path
        imagePath = "res/sprites/" + pokemon[1] + ".png";
        pokeStatArtSurface = LoadImage(&pokeStatArtSurface, imagePath);

        SDL_Rect pokemonStatArtRect;
        pokemonStatArtRect.x = 0;
        pokemonStatArtRect.y = 20;
        pokemonStatArtRect.w = pokeStatArtSurface->w * 3;
        pokemonStatArtRect.h = pokeStatArtSurface->h * 3;

        SDL_BlitScaled(pokeStatArtSurface, NULL, pokeStatsSurface, &pokemonStatArtRect);
        SDL_FreeSurface(pokeStatArtSurface);
        cout << "after:SDL_BlitScaled - pokeStatArtSurface" << endl;

        // Load & Blits pokeStatsType1Surface
        // pixel art path
        typePath1 = "res/types/" + pokemon[8] + ".png";
        pokeStatsType1Surface = LoadImage(&pokeStatsType1Surface, typePath1);

        SDL_Rect pokemonType1Rect;
        pokemonType1Rect.x = (pokemonStatArtRect.w / 2) - pokeStatsType1Surface->w * 2;
        pokemonType1Rect.y = pokemonStatArtRect.h;
        pokemonType1Rect.w = pokeStatsType1Surface->w * 2;
        pokemonType1Rect.h = pokeStatsType1Surface->h * 2;

        SDL_BlitScaled(pokeStatsType1Surface, NULL, pokeStatsSurface, &pokemonType1Rect);
        SDL_FreeSurface(pokeStatsType1Surface);
        cout << "after:SDL_BlitScaled - pokeStatsType1Surface" << endl;

        // Load & Blits pokeListType2Surface
        // pixel art path
        if (!pokemon[9].empty()) {

            typePath2 = "res/types/" + pokemon[9] + ".png";
            pokeStatsType2Surface = LoadImage(&pokeStatsType2Surface, typePath2);

            SDL_Rect pokemonType2Rect;
            pokemonType2Rect.x = pokemonStatArtRect.w / 2;
            pokemonType2Rect.y = pokemonStatArtRect.h;
            pokemonType2Rect.w = pokeStatsType2Surface->w * 2;
            pokemonType2Rect.h = pokeStatsType2Surface->h * 2;

            SDL_BlitScaled(pokeStatsType2Surface, NULL, pokeStatsSurface, &pokemonType2Rect);
            SDL_FreeSurface(pokeStatsType2Surface);
            cout << "after:SDL_BlitScaled - pokeStatsType2Surface" << endl;
        }


        //---------------------------------------------------------

        int statTotal = stoi(pokemon[2]) + stoi(pokemon[3]) + stoi(pokemon[4]) + stoi(pokemon[5]) + stoi(pokemon[6]) + stoi(pokemon[7]);

        // Create a string stream to build the stats text
        stringstream statsStream;

        string pokemonName = pokemon[1];
        pokemonName[0] = toupper(pokemonName[0]);
        // If an underscore is found, get the substring up to the underscore
        size_t underscorePos = pokemonName.find('-');
        pokemonName = underscorePos != string::npos ? pokemonName.substr(0, underscorePos) : pokemonName;
        //// Add the Pokémon's stats to the stream
        statsStream << "Name: " << pokemonName << "\n";
        statsStream << "HP: " << pokemon[2] << "\n";
        statsStream << "Attack: " << pokemon[3] << "\n";
        statsStream << "Defense: " << pokemon[4] << "\n";
        statsStream << "Sp. Atk: " << pokemon[5] << "\n";
        statsStream << "Sp. Def: " << pokemon[6] << "\n";
        statsStream << "Speed: " << pokemon[7] << "\n";
        statsStream << "Total: " << statTotal << "\n";

        // Convert the stream to a string
        string statsText = statsStream.str();

        // Render the text into an SDL_Surface
        statsFont = OpenFont(&statsFont, fontPath, 15);
        SDL_Surface* statsSurface = TTF_RenderText_Blended(
            statsFont,
            statsText.c_str(),
            color
        );

        // Define a rect for the texture
        SDL_Rect statsRect;
        statsRect.x = (WINDOW_WIDTH - 260);    // Adjust the x position according to your needs
        statsRect.y = 10;                       // Adjust the y position according to your needs
        statsRect.w = statsSurface->w;
        statsRect.h = statsSurface->h;

        vector<string> lines = splitLines(statsText);
        int lineHeight = 20; // Adjust this to match your font's height
        int statSurfaceY;
        for (int i = 0; i < lines.size(); i++) {
            SDL_Surface* lineSurface1 = TTF_RenderText_Blended(statsFont, lines[i].c_str(), color);
            //SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(renderer, lineSurface);
            SDL_Rect lineRect;
            lineRect.x = statsRect.x;
            lineRect.y = statsRect.y + i * lineHeight;
            lineRect.w = lineSurface1->w;
            lineRect.h = lineSurface1->h;

            statSurfaceY = statsRect.y + i * lineHeight;

            SDL_BlitSurface(lineSurface1, NULL, pokeStatsSurface, &lineRect);
            SDL_FreeSurface(lineSurface1);
            //SDL_DestroyTexture(lineTexture);
        }
        // Clean up
        SDL_FreeSurface(statsSurface);

    //--------------------------------------------------------------------------------------------//                    

        // Create a string stream to build the encounter stats text
        //stringstream encounterStatsStream;
        // Create a map to store the encounter data for each game
        map<string, vector<string>> gameEncounterData;

        // Iterate over the encounter data for the currently selected Pokémon
        for (int i = 0; i < encounterData.size(); i++) {
            // Get the game name and encounter location
            string gameName = encounterData[i][2];
            string encounterLocation = encounterData[i][1] + ": " + encounterData[i][3];

            // Add the encounter location to the game's list in the map
            gameEncounterData[gameName].push_back(encounterLocation);
        }

        // Create a string stream to build the encounter stats text
        stringstream encounterStatsStream;

        // Iterate over the gameEncounterData map
        for (const auto& game : gameEncounterData) {
            // Print the game name
            encounterStatsStream << game.first << ":\n";

            // Iterate over the encounter locations for this game
            for (const auto& location : game.second) {
                // Print the encounter location
                encounterStatsStream << "    " << location << "\n";
            }
        }


        // Convert the stream to a string
        string encounterStatsText = encounterStatsStream.str();
        cout << encounterStatsStream.str() << endl;

        // Render the text into an SDL_Surface 
        statsFont = OpenFont(&statsFont, fontPath, 12);
         SDL_Surface* encounterStatsSurface = TTF_RenderText_Blended(
                statsFont,
                encounterStatsText.c_str(),
                color
            );
            if (encounterStatsSurface == NULL) {
                cout << "Unable to render text! SDL Error: encounterStatsSurface" << TTF_GetError() << endl;
                exit(EXIT_FAILURE);
            }
        

        // Define a rect for the texture
        SDL_Rect encounterStatsRect;
        encounterStatsRect.x = (WINDOW_WIDTH - 350);    // Adjust the x position according to your needs
        encounterStatsRect.y = statSurfaceY + 20; // Position it under the Pokémon stats
        encounterStatsRect.w = encounterStatsSurface->w;
        encounterStatsRect.h = encounterStatsSurface->h;

        vector<string> encounterLines = splitLines(encounterStatsText);
        for (int i = 0; i < encounterLines.size(); i++) {
            SDL_Surface* lineSurface2 = TTF_RenderText_Blended(statsFont, encounterLines[i].c_str(), color);
            SDL_Rect lineRect2;
            lineRect2.x = encounterStatsRect.x;
            lineRect2.y = encounterStatsRect.y + i * lineHeight;
            lineRect2.w = lineSurface2->w;
            lineRect2.h = lineSurface2->h;

            SDL_BlitSurface(lineSurface2, NULL, pokeStatsSurface, &lineRect2);
            SDL_FreeSurface(lineSurface2);
        }
        //--------------------------------------------------------------------------------------------//  

        /*string criePath = "res/audio/cries/" + pokemon[1] + ".ogg";
        Mix_Music* sound = Mix_LoadMUS(criePath.c_str());
        if (sound == NULL) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        }

        Mix_VolumeMusic(128);
        Mix_PlayMusic(sound, -1);

        Mix_FreeMusic(sound);
        sound = NULL;*/

        //--------------------------------------------------------------------------------------------//  

        SDL_BlitSurface(pokeStatsSurface, NULL, *screen, &pokeStatsRect);
        SDL_FreeSurface(pokeStatsSurface);

        cout << "ending: RenderPokedexStats" << endl;

        needsRendering = false;
    };
}

int main(int argc, char* argv[])
{
    SDL_Window* window = NULL;
    SDL_Surface* screen = NULL;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font = NULL;

    // Initialization code
    InitializeSDL(&window, &renderer, &texture, &screen, &font);

    const string SQL_STRING_GET_POKEMONS_BASIC_STATS =
        "SELECT "
        "p.id, "
        "p.identifier AS pokemon_identifier, "
        "MAX(CASE WHEN ps.stat_id = 1 THEN ps.base_stat END) AS hp, "
        "MAX(CASE WHEN ps.stat_id = 2 THEN ps.base_stat END) AS attack, "
        "MAX(CASE WHEN ps.stat_id = 3 THEN ps.base_stat END) AS defense, "
        "MAX(CASE WHEN ps.stat_id = 4 THEN ps.base_stat END) AS special_attack, "
        "MAX(CASE WHEN ps.stat_id = 5 THEN ps.base_stat END) AS special_defense, "
        "MAX(CASE WHEN ps.stat_id = 6 THEN ps.base_stat END) AS speed, "
        "COALESCE(MAX(CASE WHEN pt.slot = 1 THEN t.identifier END), '') AS type1, "
        "COALESCE(MAX(CASE WHEN pt.slot = 2 THEN t.identifier END), '') AS type2 "
        "FROM "
        "pokemon p "
        "JOIN "
        "pokemon_stats ps ON p.id = ps.pokemon_id "
        "LEFT JOIN "
        "pokemon_types pt ON p.id = pt.pokemon_id "
        "LEFT JOIN "
        "types t ON pt.type_id = t.id "
        "WHERE "
        "p.id AND "
        "p.identifier IS NOT NULL "
        "GROUP BY "
        "p.id, p.identifier "
        "ORDER BY "
        "p.id "
        "LIMIT 649;";

    const string SQL_STRING_GET_POKEMONS_ENCOUNTER_STATS = R"(
        SELECT
            pokemon_species_names.pokemon_species_id,
            location_names.name AS location_name,
            version_names.name AS game_name,
            encounter_method_prose.name AS encounter_method,
            GROUP_CONCAT(CAST(encounter_slots.rarity AS TEXT), ',') AS encounter_chances
        FROM 
            encounters
        JOIN 
            pokemon_species_names ON encounters.pokemon_id = pokemon_species_names.pokemon_species_id
        JOIN 
            location_area_encounter_rates ON encounters.location_area_id = location_area_encounter_rates.location_area_id
        JOIN 
            location_areas ON location_area_encounter_rates.location_area_id = location_areas.id
        JOIN 
            locations ON location_areas.location_id = locations.id
        JOIN 
            location_names ON locations.id = location_names.location_id
        JOIN 
            version_names ON location_area_encounter_rates.version_id = version_names.version_id
        JOIN 
            encounter_slots ON encounters.encounter_slot_id = encounter_slots.id
        JOIN 
            encounter_method_prose ON encounter_slots.encounter_method_id = encounter_method_prose.encounter_method_id
        WHERE 
            location_names.local_language_id = 9 AND 
            version_names.local_language_id = 9 AND 
            encounter_method_prose.local_language_id = 9 AND 
            pokemon_species_names.local_language_id = 9 AND
            pokemon_species_names.pokemon_species_id BETWEEN 1 AND 649
        GROUP BY 
            pokemon_species_names.pokemon_species_id,
            location_name,
            game_name,
            encounter_method
        ORDER BY 
            pokemon_species_names.pokemon_species_id
        )";


    //cout << "executeSQL(SQL_STRING_GET_POKEMONS) \n";
    vector<vector<string>> results = executeSQL(SQL_STRING_GET_POKEMONS_BASIC_STATS);
    vector<vector<string>> encounterResults = executeSQL(SQL_STRING_GET_POKEMONS_ENCOUNTER_STATS);
    vector<int> nextGenID = { 0, 151, 251, 386, 493 };
    int currentGenIndex = 0; // Initialize this to the starting index

    // Create a map to store the encounter data for each Pokémon
    map<int, vector<vector<string>>> pokemonEncounterData;

    // Iterate over the encounterResults vector
    for (int i = 0; i < encounterResults.size(); i++) {
        // Get the encounter data for the current Pokémon
        vector<string> encounterData = encounterResults[i];

        // Get the Pokémon ID from the encounter data
        int pokemonId = stoi(encounterData[0]);
        // Add the encounter data to the map
        pokemonEncounterData[pokemonId].push_back(encounterData);
    }
    
    // Iterate over the map
    /*for (const auto& pair : pokemonEncounterData) {
        // Print the Pokémon ID
        std::cout << "Pokémon ID: " << pair.first << std::endl;

        // Iterate over the vector of encounter data for this Pokémon
        for (const auto& encounterData : pair.second) {
            // Print the encounter data
            for (const auto& data : encounterData) {
                std::cout << data << " | ";
            }
            std::cout << std::endl;
        }
        std::cout << "------------------------" << std::endl;
    } */

    // Main loop    
    int topItem = 0; // The index of the top item on the screen
    int selectedItem = 0;
    int maxListIDWidth = 0;
    int maxListLabelHight = 0;
    View currentView = View::LIST;
    SDL_Event windowEvent;
    bool quit = false;
    bool needsRendering = true;
    while (!quit) {
        HandleInputs(windowEvent, quit, currentView, selectedItem, topItem, currentGenIndex, nextGenID, needsRendering);

        if (currentView == View::LIST) {
            // Clear the window and make it all white
            SDL_RenderClear(renderer);


            // Rendering based on the current view
            RenderPokedexList(&screen, &renderer, &font, results, topItem, selectedItem, maxListIDWidth, maxListLabelHight, needsRendering);

        }
        else if (currentView == View::STATS) {

            // Clear the window and make it all white
            SDL_RenderClear(renderer);

            //cout << "Size of pokemonEncounterData: " << pokemonEncounterData.size() << endl;
            //cout << "selectedItem: " << selectedItem << endl;
            //cout << "Size of pokemonEncounterData[selectedItem]: " << pokemonEncounterData[selectedItem+1].size() << endl;

            vector<vector<string>> encounterData = pokemonEncounterData[selectedItem+1];

            if (pokemonEncounterData[selectedItem + 1].size() == 0) {
                encounterData = pokemonEncounterData[selectedItem];
                    if (pokemonEncounterData[selectedItem].size() == 0) {
                        encounterData = pokemonEncounterData[selectedItem-1];                     
                    };
            }
            // Iterate over the outer vector
            /*for (const auto& result : encounterData) {
                // Iterate over the inner vector
                for (const auto& element : result) {
                    std::cout << "element" << " | ";
                    std::cout << element << " | ";
                }
                std::cout << std::endl;
            */
           
            
            // Generate Stats for selected Pokemon
            RenderPokedexStats(&screen, &renderer, &font, results, encounterData, topItem, selectedItem, maxListIDWidth, maxListLabelHight, needsRendering);
        }

        // Main loop continuation
        // Flip the backbuffer
        SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

    }
    // Cleanup code
    CleanupSDL(window, renderer, texture, screen);

    return EXIT_SUCCESS;
};