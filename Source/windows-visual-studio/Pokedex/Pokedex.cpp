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

static int callback(void* results, int argc, char** argv, char** azColName) {
    vector<string> row;
    /*for (int i = 0; i < argc; i++) {
        if (argv[i] != nullptr) {
            row.push_back(argv[i]);
        }
        else {
            row.push_back("");  // or some other value indicating NULL
        }
    }*/
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
            case SDLK_RIGHT:
                currentView = View::STATS;
                needsRendering = true;
                break;
            case SDLK_LEFT:
                currentView = View::LIST;
                needsRendering = true;
                break;
            case SDLK_UP:
                needsRendering = true;
                if (selectedItem > 0) selectedItem--;
                if (selectedItem < topItem) topItem = selectedItem;
                break;
            case SDLK_DOWN:
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
SDL_Surface* LoadImage(SDL_Surface** imgSurafce, string& imagePath) {
    cout << "starting: LoadImage" << endl;
    SDL_Surface* optimizedSurface = NULL;
    *imgSurafce = IMG_Load(imagePath.c_str());
    if (*imgSurafce == NULL) {
        cout << "Unable to load image! SDL Error: " << IMG_GetError() << endl;
        exit(EXIT_FAILURE);
    }
    optimizedSurface = SDL_ConvertSurfaceFormat(*imgSurafce, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(*imgSurafce);
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
    SDL_Surface* pokeListIDSurface = NULL, *maxListIDSurfaceWidth = NULL;
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

    !SDL_BlitSurface(pokeListIDSurface, NULL, *pokemonListEntrySurface, &pokeListIDRect);
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


    //int statTotal = stoi(pokemon[2]) + stoi(pokemon[3]) + stoi(pokemon[4]) + stoi(pokemon[5]) + stoi(pokemon[6]) + stoi(pokemon[7]);
    //statsStream << left << setw(5) << pokemonName << " ";
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

    //vector<string> lines = splitLines(statsText);
    //int lineHeight = 35; // Adjust this to match your font's height
    //int padding = 50;
    //for (int i = 0; i < lines.size(); i++) {
    pokeListStatsSurface = TTF_RenderText_Blended(
        *font,
        statsText.c_str(),
        counter == 0 ? highlightColor : color
    );
    if (pokeListStatsSurface == NULL) {
        cout << "Unable to render text! SDL Error: " << TTF_GetError() << endl;
        exit(EXIT_FAILURE);
    }
    /*pokeListStatsSurface = RenderText(
        pokeListStatsSurface,
        statsText.c_str(),
        *font,
        color
    );*/

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
        /*
        SDL_Rect pokeListLabelEntry;
        pokeListLabelEntry.x = 0;
        pokeListLabelEntry.y = 0;
        pokeListLabelEntry.w = pokeListLabelSurface->w;
        pokeListLabelEntry.h = pokeListLabelSurface->h;
        */

        
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
            int colorIndex = (pokemonID - 1 ) % 2; // This will alternate between 0 and 1            
            SDL_Color rainbowColors = {
                static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][0]),
                static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][1]),
                static_cast<unsigned char>(color_map.at(pokemonType)[colorIndex][2])
            };

            // Get the number of colors
            //int numColors = sizeof(rainbowColors) / sizeof(SDL_Color);

            // Use modulo to cycle through the colors
            //SDL_Color background_color = rainbowColors[i % numColors];
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
void RenderPokedexStats(SDL_Surface** screen, SDL_Renderer** renderer, TTF_Font** font, vector<vector<string>>& results, int& topItem, int& selectedItem, int& maxListIDWidth, int& maxListLabelHight, bool& needsRendering) {
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

        // Get the number of colors
        //int numColors = sizeof(rainbowColors) / sizeof(SDL_Color);

        // Use modulo to cycle through the colors
        //SDL_Color background_color = rainbowColors[i % numColors];
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
        pokemonType1Rect.x = (pokemonStatArtRect.w / 2 ) - pokeStatsType1Surface->w * 2 ;
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


        // Create a string stream to build the stats text
        //stringstream statsStream;


        int statTotal = stoi(pokemon[2]) + stoi(pokemon[3]) + stoi(pokemon[4]) + stoi(pokemon[5]) + stoi(pokemon[6]) + stoi(pokemon[7]);
        /*/statsStream << left << setw(5) << pokemonName << " ";
        statsStream << right << setw(4) << pokemon[1];  //  speed special 
        statsStream << right << setw(4) << pokemon[2];  //  hp
        statsStream << right << setw(4) << pokemon[3];  //  attack
        statsStream << right << setw(4) << pokemon[4];  //  defense
        statsStream << right << setw(4) << pokemon[5];  //  special - attack
        statsStream << right << setw(4) << pokemon[6];  //  special - defense
        statsStream << right << setw(4) << pokemon[7];  //  speed special 
        */

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
        statsFont = OpenFont(&statsFont, fontPath, 20);
        SDL_Surface* statsSurface = TTF_RenderText_Blended(
            statsFont, 
            statsText.c_str(),
            color
        ); 

        // Convert the surface to a texture
        //SDL_Texture* statsTexture = SDL_CreateTextureFromSurface(renderer, statsSurface);

        // Define a rect for the texture
        SDL_Rect statsRect;
        statsRect.x = pokemonStatArtRect.w;    // Adjust the x position according to your needs
        statsRect.y = 50;                       // Adjust the y position according to your needs
        statsRect.w = statsSurface->w;
        statsRect.h = statsSurface->h;

        vector<string> lines = splitLines(statsText);
        int lineHeight = 50; // Adjust this to match your font's height
        for (int i = 0; i < lines.size(); i++) {
            SDL_Surface* lineSurface = TTF_RenderText_Blended(statsFont, lines[i].c_str(), color);
            //SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(renderer, lineSurface);
            SDL_Rect lineRect;
            lineRect.x = statsRect.x;
            lineRect.y = statsRect.y + i * lineHeight;
            lineRect.w = lineSurface->w;
            lineRect.h = lineSurface->h;

            SDL_BlitSurface(lineSurface, NULL, pokeStatsSurface, &lineRect);
            SDL_FreeSurface(lineSurface);
            //SDL_DestroyTexture(lineTexture);
        }
        // Clean up
        SDL_FreeSurface(statsSurface);
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

    const string SQL_STRING_GET_POKEMONS =
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

    //cout << "executeSQL(SQL_STRING_GET_POKEMONS) \n";
    vector<vector<string>> results = executeSQL(SQL_STRING_GET_POKEMONS);
    vector<int> nextGenID = { 0, 151, 251, 386, 493 };
    int currentGenIndex = 0; // Initialize this to the starting index

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

            // Generate Stats for selected Pokemon
            // call PokemonStatsView function to generate Stats for selected Pokemon 
            //PokemonStatsView(screen, font, selectedItem, maxListTextWidth);
            RenderPokedexStats(&screen, &renderer, &font, results, topItem, selectedItem, maxListIDWidth, maxListLabelHight, needsRendering);
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
}