// Pokedex.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WINDOW_HEIGHT 480                   // window height in pixels
#define WINDOW_WIDTH 640                    // window width in pixels
#define DEPTH 16                            // window depth in pixels
#define POKEMON_LIST_SIZE 649               // Total pokemon to load ( up to gen 5 )
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream> 
#include <SDL.h>                       // required for SDL operations
#include <SDL_image.h>                   // required for SDL text rendering operations
#include <SDL_ttf.h>
#include "sqlite/sqlite3.h"
using namespace std;

vector<vector<string>> results;             // Declare results vector outside of main
vector<string> splitLines(const string& text) {
    vector<string> lines;
    stringstream ss(text);
    string line;
    while (getline(ss, line, '\n')) {
        lines.push_back(line);
    }
    return lines;
};

static int callback(void* data, int argc, char** argv, char** azColName) {
    results.emplace_back(argv, argv + argc); // Use the global results vector
    return 0;
};

// Function to execute a SQL query and return the result as a string
std::string executeSQL(const std::string& sql) {
    vector<vector<string>> results;
    sqlite3* db;
    int rc;
    char* zErrMsg = 0;

    std::string result;
    if (sqlite3_exec(db, sql.c_str(), callback, &result, &zErrMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return result;
}

void PokemonPixelArt(SDL_Renderer* renderer, SDL_Surface* screen, SDL_Texture* texture, int& selectedItem, int& maxListTextWidth) {
    /*const char* SQL_STRING_GET_POKEMONS = "SELECT pokemon.identifier, types.identifier FROM pokemon JOIN pokemon_types ON pokemon.id = pokemon_types.pokemon_id JOIN types ON pokemon_types.type_id = types.id";
    sqlite3* db;
    int rc;
    char* zErrMsg = 0;
    rc = sqlite3_exec(
        db,
        SQL_STRING_GET_POKEMONS,
        callback,
        NULL,
        &zErrMsg
    );
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    else {
        cout << "Operation done successfully" << endl;
    };*/
    results[selectedItem][1][0] = tolower(results[selectedItem][1][0]);
    string imagePath = "resources/full_pic/" + results[selectedItem][1] + ".png";
    string typePath1 = "resources/types/" + results[selectedItem][3] + ".png"; // Assuming the 4th column is the primary type
    string typePath2;
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* pokemonImage = IMG_Load(imagePath.c_str());
    if (pokemonImage == NULL) {
        cout << "Unable to IMG_Load! SDL Error: " << IMG_GetError() << endl;
        exit;
    }
    SDL_Surface* typeImage1 = IMG_Load(typePath1.c_str());
    if (typeImage1 == NULL) {
        cout << "Unable to IMG_Load! SDL Error: " << IMG_GetError() << endl;
        exit;
    }
    optimizedSurface = SDL_ConvertSurfaceFormat(pokemonImage, SDL_PIXELFORMAT_RGBA32, 0);
    if (optimizedSurface == NULL) {
        cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << endl;
    }
    //SDL_Texture* pokemonTexture = SDL_CreateTextureFromSurface(renderer, pokemonImage);

    float aspectRatio = (float)typeImage1->w / typeImage1->h;
    int typeRectHeight = (WINDOW_WIDTH - maxListTextWidth - 100) / aspectRatio; // Adjust the width to match the Pokémon image

    // Define a rect for the surface
    SDL_Rect pokemonDestRect;
    pokemonDestRect.x = 0;                                      // Adjust the x position according to your needs
    pokemonDestRect.y = 0;                                      // Adjust the y position according to your needs
    pokemonDestRect.w = (WINDOW_WIDTH - maxListTextWidth) - 100;      // Adjust the width to fill the space to the left of the list
    pokemonDestRect.h = (WINDOW_WIDTH - maxListTextWidth) - 100;      // Adjust the height to fill the window height

    SDL_FreeSurface(pokemonImage);
    pokemonImage = optimizedSurface;
    SDL_BlitScaled(pokemonImage, NULL, screen, &pokemonDestRect);
    SDL_FreeSurface(pokemonImage);


    optimizedSurface = SDL_ConvertSurfaceFormat(typeImage1, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_Rect typeRect1;
    typeRect1.x = 0; // Same x position as the Pokémon image
    typeRect1.y = pokemonDestRect.h; // Position it below the Pokémon image
    typeRect1.w = typeImage1->w * 3; // Use the width of the type image
    typeRect1.h = typeImage1->h * 3; // Use the height of the type image

    SDL_FreeSurface(typeImage1);
    typeImage1 = optimizedSurface;
    SDL_BlitScaled(typeImage1, NULL, screen, &typeRect1);
    SDL_FreeSurface(typeImage1);

    // Check if pokemon has second type, and render it to screen. 
    SDL_Surface* typeImage2 = NULL;
    if (results[selectedItem][4] != " ") {
        typePath2 = "resources/types/" + results[selectedItem][4] + ".png"; // Assuming the 4th column is the primary type

        typeImage2 = IMG_Load(typePath2.c_str());
        if (typeImage2 == NULL) {
            cout << "IMG_Load: " << IMG_GetError() << endl;
            exit;
        }

        optimizedSurface = SDL_ConvertSurfaceFormat(typeImage2, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_Rect typeRect2;
        typeRect2.x = typeRect1.x + typeRect1.w; // Same x position as the Pokémon image
        typeRect2.y = pokemonDestRect.h; // Position it below the first type image
        typeRect2.w = typeImage2->w * 3; // Use the width of the type image
        typeRect2.h = typeImage2->h * 3; // Use the height of the type image

        SDL_FreeSurface(typeImage2);
        typeImage2 = optimizedSurface;
        SDL_BlitScaled(typeImage2, NULL, screen, &typeRect2);
        SDL_FreeSurface(typeImage2);
    }
};

void MakeListLabel(SDL_Surface* screen, TTF_Font* font, int& selectedItem, int& maxListTextWidth) {
    string labelText = "ID No. Name";
    SDL_Color color = { 255, 255, 255 };

    // Render the text into an SDL_Surface
    SDL_Surface* labelSurface = TTF_RenderText_Blended(
        font,
        labelText.c_str(),
        color
    );
    if (labelSurface == NULL) {
        cout << "TTF_RenderText_Blended Error: " << TTF_GetError() << endl;
        exit;
    };

    SDL_Rect labelRect;
    labelRect.x = (WINDOW_WIDTH - maxListTextWidth) - 40; // Same x position as the list
    labelRect.y = 0; // Position it at the top
    labelRect.w = labelSurface->w;
    labelRect.h = labelSurface->h;

    SDL_BlitSurface(labelSurface, NULL, screen, &labelRect);
    SDL_FreeSurface(labelSurface);
};

void PokemonStatsView(SDL_Surface* screen, TTF_Font* font, int& selectedItem, int& maxListTextWidth) {
    /*string SQL_STRING_GET_POKEMONS = "SELECT base_stat FROM pokemon_stats WHERE pokemon_id = " + std::to_string(selectedItem);
    sqlite3* db;
    int rc;
    char* zErrMsg = 0;
    rc = sqlite3_exec(
        db,
        SQL_STRING_GET_POKEMONS.c_str(),
        callback,
        NULL,
        &zErrMsg
    );
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    else {
        cout << "Operation done successfully" << endl;
    };*/

    // Loop through the results vector
    for (int i = 0; i < results.size(); i++) {
        // Get the selected Pokémon's stats from the results vector
        vector<string> selectedPokemon = results[selectedItem];
        SDL_Color color = { 255, 255, 255 };


        size_t underscorePos = selectedPokemon[1].find('-');
        selectedPokemon[1][0] = toupper(selectedPokemon[1][0]);
        // If an underscore is found, get the substring up to the underscore
        string pokemonName = underscorePos != string::npos ? selectedPokemon[1].substr(0, underscorePos) : selectedPokemon[1];

        // Create a string stream to build the stats text
        stringstream statsStream;

        // Add the Pokémon's stats to the stream
        selectedPokemon[1][0] = toupper(selectedPokemon[1][0]);
        statsStream << left << setw(4) << "Name: " << left << setw(5) << pokemonName << "\n";
        statsStream << left << setw(7) << "HP:     " << right << setw(5) << selectedPokemon[6] << "\n";
        statsStream << left << setw(7) << "Attack: " << right << setw(5) << selectedPokemon[7] << "\n";
        statsStream << left << setw(7) << "Defense:" << right << setw(5) << selectedPokemon[8] << "\n";
        statsStream << left << setw(7) << "Sp. Atk:" << right << setw(5) << selectedPokemon[9] << "\n";
        statsStream << left << setw(7) << "Sp. Def:" << right << setw(5) << selectedPokemon[10] << "\n";
        statsStream << left << setw(7) << "Speed:  " << right << setw(5) << selectedPokemon[11] << "\n";
        statsStream << left << setw(7) << "Total:  " << right << setw(5) << selectedPokemon[5] << "\n";

        // Convert the stream to a string
        string statsText = statsStream.str();

        // Render the text into an SDL_Surface
        SDL_Surface* statsSurface = NULL;

        vector<string> lines = splitLines(statsText);
        int lineHeight = 35; // Adjust this to match your font's height
        int padding = 50;
        for (int i = 0; i < lines.size(); i++) {
            statsSurface = TTF_RenderText_Blended(font, lines[i].c_str(), color);
            SDL_Rect statsRect;
            statsRect.x = (WINDOW_WIDTH - maxListTextWidth) - padding;
            statsRect.y = padding + i * lineHeight;
            statsRect.w = statsSurface->w;
            statsRect.h = statsSurface->h;
            // Clean up
            SDL_BlitSurface(statsSurface, NULL, screen, &statsRect);
            SDL_FreeSurface(statsSurface);
        }
    }
};

void PokemonFullList(SDL_Surface* screen, TTF_Font* font, int& selectedItem, int& maxListTextWidth) {
    // Render each Pokemon name
    //Make label
    MakeListLabel(screen, font, selectedItem, maxListTextWidth);
    for (int i = selectedItem; i < results.size() && i < selectedItem + WINDOW_HEIGHT; i++) { // Adjust the 24 according to your font height
        SDL_Surface* textListSurface = NULL;
        ostringstream textStream;
        SDL_Color color = { 255, 255, 255 };
        SDL_Color highlightColor = { 255, 0, 0 };

        size_t underscorePos = results[i][1].find('-');
        results[i][1][0] = toupper(results[i][1][0]);
        // If an underscore is found, get the substring up to the underscore
        string pokemonName = underscorePos != string::npos ? results[i][1].substr(0, underscorePos) : results[i][1];
        textStream << right << setw(4) << results[i][0] << " " << pokemonName; // setw(4) assumes Pokemon ID is up to 3 digits.
        string text = textStream.str();

        // Render the text into an SDL_Surface
        textListSurface = TTF_RenderText_Blended(
            font,
            text.c_str(),
            i == selectedItem ? highlightColor : color
        );
        textListSurface->w > maxListTextWidth ? maxListTextWidth = textListSurface->w : maxListTextWidth;
        int labelHeight = textListSurface->h;
        // Define a rect for the texture             
        SDL_Rect textRect;
        textRect.x = (WINDOW_WIDTH - maxListTextWidth) - 10; // A small margin from the left
        textRect.y = (i - selectedItem) * 24 + labelHeight;  // Adjust the y position for each item
        textRect.w = textListSurface->w;
        textRect.h = textListSurface->h;

        SDL_BlitSurface(textListSurface, NULL, screen, &textRect);
        SDL_FreeSurface(textListSurface);
    }
};



enum class View {
    LIST,
    STATS
};
int main(int argc, char* argv[])
{
    char* zErrMsg = 0;
    SDL_Surface* screen = NULL;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer = NULL;
    string SQL_STRING_GET_POKEMONS = "SELECT * FROM pokemon";

    sqlite3* db;
    int rc;
    // Open DP for Stats
    rc = sqlite3_open("pokedexStats.sqlite", &db);
    if (rc != SQLITE_OK) {
        cout << "SQLite coult not open file! SQLite Error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return EXIT_FAILURE;
    }
    else {
        cout << "SQLite Opened file! " << endl;
    };
    rc = sqlite3_exec(
        db,
        SQL_STRING_GET_POKEMONS.c_str(),
        callback,
        NULL,
        &zErrMsg
    );
    if (rc != SQLITE_OK) {
        cout << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
    else {
        cout << "Operation done successfully" << endl;
    };
    // Now you can use the results vector outside of the sqlite3_exec call
    for (const auto& row : results) {     // for each row in results
        for (const auto& col : row) {     // for each column in a row
            cout << col;                  // do something..
        };
        printf("\n");
    };

    // Use SDL_Init() to Initialize SDL components that we will need for our program. 
    // Use SDL_GetError() to get any errors
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL coult not initialize! SDL Error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    };

    // Initialize SDL_ttf library
    if (TTF_Init() == -1) {
        cout << "SDL coult not initialize TTF_Init: " << TTF_GetError() << endl;
        return EXIT_FAILURE;
    }

    // Create a window with the specified position, dimensions, and flags.
    SDL_Window* window = SDL_CreateWindow(
        "Pokedex",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Surface* blackSurface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, DEPTH, 0, 0, 0, 0);
    SDL_FillRect(blackSurface, NULL, SDL_MapRGB(blackSurface->format, 0, 0, 0));

    // Create a surface with the specified position, dimensions, and flags.
    screen = SDL_CreateRGBSurface(
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        DEPTH,
        0, 0, 0, 0
    );
    // Perform a fast fill of a rectangle ( our surface ) with a specific color.
    SDL_FillRect(
        screen,
        &screen->clip_rect,
        SDL_MapRGB(screen->format, 0x00, 0x00, 0x00)
    );

    // Prepare Text Msg for displaying on screen
    // Load a font
    TTF_Font* font = TTF_OpenFont("Pokemon_GB.ttf", 20);
    if (font == NULL) {
        cout << "TTF_OpenFont: " << TTF_GetError() << endl;
        return EXIT_FAILURE;
    }
    // Set the color of the text
    SDL_Color color = { 255, 255, 255 };


    // Initialize SDL_image with PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }

    // Variables to handle navigation
    int selectedItem = 0;
    bool quit = false;
    int topItem = 0; // The index of the top item on the screen
    int maxListTextWidth = 0;
    View currentView = View::LIST;
    SDL_Event windowEvent;
    while (!quit) {

        while (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT) {
                quit = true;
            }
            else if (windowEvent.type == SDL_KEYDOWN) {
                switch (windowEvent.key.keysym.sym) {
                case SDLK_RIGHT:
                    currentView = View::STATS;
                    break;
                case SDLK_LEFT:
                    currentView = View::LIST;
                    break;
                case SDLK_UP:
                    if (selectedItem > 0) selectedItem--;
                    if (selectedItem < topItem) topItem = selectedItem;
                    break;
                case SDLK_DOWN:
                    if (selectedItem < 649 ) selectedItem++; // 649 = number of pokemons up to gen 5
                    if (selectedItem >= topItem) topItem++; // Adjust the 30 according to your font height
                    break;
                case SDLK_ESCAPE: // Add this case for the ESC key
                    quit = true;
                    break;
                default:
                    break;
                }
            }
        }

        // Clear the window and make it all white
        SDL_FillRect(
            screen,
            &screen->clip_rect,
            SDL_MapRGB(screen->format, 0x00, 0x00, 0x00)
        );
        // Clear the window and make it all white
        SDL_RenderClear(renderer);

        // Generate Pokemon Image
        // call PokemonPixelArt function to generate Pokemon Image
        PokemonPixelArt(renderer, screen, texture, selectedItem, maxListTextWidth);
        //SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);

        if (currentView == View::LIST) {
            // Clear the window and make it all white
            SDL_RenderClear(renderer);

            /*const char* SQL_STRING_GET_POKEMONS = "SELECT * FROM pokemon LIMIT 649";
            rc = sqlite3_exec(
                db,
                SQL_STRING_GET_POKEMONS,
                callback,
                NULL,
                &zErrMsg
            );
            if (rc != SQLITE_OK) {
                cout << "SQL error: " << zErrMsg << endl;
                sqlite3_free(zErrMsg);
            }
            else {
                cout << "Operation done successfully" << endl;
            };*/

            // Generate Full Pokemon List
            //call PokemonFullList function to generate full list of pokemon    
            PokemonFullList(screen, font, selectedItem, maxListTextWidth);
            //SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
        }
        else if (currentView == View::STATS) {
            // Clear the window and make it all white
            SDL_RenderClear(renderer);

            // Generate Stats for selected Pokemon
            // call PokemonStatsView function to generate Stats for selected Pokemon 
            PokemonStatsView(screen, font, selectedItem, maxListTextWidth);
        }

        // Flip the backbuffer
        SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    sqlite3_close(db);
    SDL_Quit();
    return EXIT_SUCCESS;
};


//#define WINDOW_HEIGHT 480                   // window height in pixels
//#define WINDOW_WIDTH 640                    // window width in pixels
//#define DEPTH 16                            // window depth in pixels
//#include "sqlite/sqlite3.h"					// sqlite library
//    //#include <nlohmann/json.hpp>				// json library
//#include <iostream>
//#include <iomanip>
//#include <string>
//#include <vector>
//#include <sstream> 
//#include <SDL2/SDL.h>                       		
//#include <SDL2/SDL_image.h>                   	
//#include <SDL2/SDL_ttf.h>
//    using namespace std;
//
//    enum class View {
//        LIST,
//        STATS
//    };
//
//    void InitializeSDL(SDL_Window * *window, SDL_Renderer * *renderer, SDL_Texture * *texture, SDL_Surface * *screen) {
//        // Use SDL_Init() to Initialize SDL components that we will need for our program. 
//        // Use SDL_GetError() to get any errors
//        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
//            cout << "SDL coult not initialize! SDL Error: " << SDL_GetError() << endl;
//            exit;
//        };
//
//        // Create a window with the specified position, dimensions, and flags.
//        *window = SDL_CreateWindow(
//            "Pokedex",
//            SDL_WINDOWPOS_UNDEFINED,
//            SDL_WINDOWPOS_UNDEFINED,
//            WINDOW_WIDTH,
//            WINDOW_HEIGHT,
//            SDL_WINDOW_SHOWN
//        );
//
//        *renderer = SDL_CreateRenderer(
//            *window,
//            -1,
//            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
//        );
//
//        *texture = SDL_CreateTexture(
//            *renderer,
//            SDL_PIXELFORMAT_RGB565,
//            SDL_TEXTUREACCESS_STREAMING,
//            WINDOW_WIDTH,
//            WINDOW_HEIGHT
//        );
//
//        // Create a surface with the specified position, dimensions, and flags.
//        *screen = SDL_CreateRGBSurface(
//            0,
//            WINDOW_WIDTH,
//            WINDOW_HEIGHT,
//            DEPTH,
//            0, 0, 0, 0
//        );
//
//        // Initialize SDL_ttf library
//        if (TTF_Init() == -1) {
//            cout << "SDL coult not initialize TTF_Init: " << TTF_GetError() << endl;
//            exit;
//        }
//
//        // Initialize SDL_image with PNG loading
//        int imgFlags = IMG_INIT_PNG;
//        if (!(IMG_Init(imgFlags) & imgFlags)) {
//            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
//            exit;
//        }
//
//        // Perform a fast fill of a rectangle ( our surface ) with a specific color.
//        SDL_FillRect(
//            *screen,
//            &(*screen)->clip_rect,
//            SDL_MapRGB((*screen)->format, 0x00, 0x00, 0x00)
//        );
//    };
//
//    static int callback(void* results, int argc, char** argv, char** azColName) {
//        vector<string> row;
//        /*for (int i = 0; i < argc; i++) {
//            if (argv[i] != nullptr) {
//                row.push_back(argv[i]);
//            }
//            else {
//                row.push_back("");  // or some other value indicating NULL
//            }
//        }*/
//        static_cast<vector<vector<string>>*>(results)->emplace_back(argv, argv + argc);
//        return 0;
//    };
//
//    // Function to execute a SQL query and return the result as a string
//    vector<vector<string>> executeSQL(const string & sql) {
//        vector<vector<string>> results;
//        sqlite3* db;
//        int rc;
//        char* zErrMsg = 0;
//        rc = sqlite3_open("pokedex.sqlite", &db);
//        cout << "inside callback: if" << endl;
//        if (rc != SQLITE_OK) {
//            cout << "SQLite coult not open file! SQLite Error: " << sqlite3_errmsg(db) << endl;
//            sqlite3_close(db);
//            return results;
//        }
//        else {
//            cout << "inside callback: else" << endl;
//            if (sqlite3_exec(db, sql.c_str(), callback, &results, &zErrMsg) != SQLITE_OK) {
//
//                fprintf(stderr, "SQL error: %s\n", zErrMsg);
//                sqlite3_free(zErrMsg);
//
//            }
//            sqlite3_close(db);
//            return results;
//        }
//    };
//
//    vector<string> splitLines(const string & text) {
//        vector<string> lines;
//        stringstream ss(text);
//        string line;
//        while (getline(ss, line, '\n')) {
//            lines.push_back(line);
//        }
//        return lines;
//    };
//
//    void pokemonID(vector<string>&pokemon, SDL_Surface * *pokemonListEntrySurface, TTF_Font * *font, bool& isSelected, int& maxListTextWidth) {
//
//        SDL_Surface* pokeListIDSurface = NULL;
//        SDL_Color color = { 255, 255, 255 };
//        SDL_Color highlightColor = { 255, 0, 0 };
//
//        pokeListIDSurface = TTF_RenderText_Blended(
//            *font,
//            pokemon[0].c_str(),
//            isSelected ? highlightColor : color
//        );
//
//        pokeListIDSurface->w > maxListTextWidth ? maxListTextWidth = pokeListIDSurface->w : maxListTextWidth;
//        // Define a rect for the texture             
//        SDL_Rect pokeListIDRect;
//        pokeListIDRect.x = 0; // A small margin from the left
//        pokeListIDRect.y = 0;  // Adjust the y position for each item
//        pokeListIDRect.w = (*pokemonListEntrySurface)->w * 0.1;
//        pokeListIDRect.h = (*pokemonListEntrySurface)->h;
//
//
//
//        SDL_BlitSurface(pokeListIDSurface, NULL, *pokemonListEntrySurface, &pokeListIDRect);
//    }
//
//    void pokemonListPixelArt(SDL_Surface * *pokemonListEntrySurface, int& counter, int& selectedItem, int& maxListTextWidth) {
//        // Get pixel art
//        string imagePath, typePath1, typePath2;
//        SDL_Surface* optimizedSurface = NULL, * pokeListArtSurface = NULL, * pokeListType1Surface = NULL, * pokeListType2Surface = NULL;
//        vector<vector<string>> results;
//        // Get all * from pokemon Table
//        const std::string SQL_STRING_GET_POKEMONS_AND_TYPES =
//            "SELECT p.id, p.identifier, "
//            "COALESCE(MAX(CASE WHEN pt.slot = 1 THEN t.identifier END), '') AS type1, "
//            "COALESCE(MAX(CASE WHEN pt.slot = 2 THEN t.identifier END), '') AS type2 "
//            "FROM (SELECT * FROM pokemon ORDER BY id LIMIT 649) p "
//            "LEFT JOIN pokemon_types pt ON p.id = pt.pokemon_id "
//            "LEFT JOIN types t ON pt.type_id = t.id "
//            "WHERE p.identifier IS NOT NULL "
//            "GROUP BY p.id";
//
//        results = executeSQL(SQL_STRING_GET_POKEMONS_AND_TYPES);
//        vector<string> pokemon = results[counter];
//
//        // Get pokemon image pixel art
//        imagePath = "resources/full_pic/" + pokemon[1] + ".png";
//        typePath1 = "resources/types/" + pokemon[2] + ".png";                     // Assuming the 4th column is the primary type
//        cout << "imagePath : " << imagePath << endl;
//        cout << "typePath1 : " << typePath1 << endl;
//        /*int y = 0;
//        for (const auto& row : pokemon) {     // for each row in results
//            if (y >= 648) break;
//            printf("\n");
//            for (const auto& col : row) {     // for each column in a row
//                cout << col;                  // do something..
//            };
//            y++;
//        };*/
//        ////------------------------------------------------------------////////////
//            // Create pokemon image pixel art surface
//        pokeListArtSurface = IMG_Load(imagePath.c_str());
//        if (pokeListArtSurface == NULL) {
//            cout << "Unable to IMG_Load! SDL Error: " << IMG_GetError() << endl;
//            exit;
//        }
//        optimizedSurface = SDL_ConvertSurfaceFormat(pokeListArtSurface, SDL_PIXELFORMAT_RGBA32, 0);
//        if (optimizedSurface == NULL) {
//            cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << endl;
//            exit;
//        }
//
//        // Define a rect for the surface
//        SDL_Rect pokemonArtRect;
//        pokemonArtRect.x = (*pokemonListEntrySurface)->w * 0.1;                     // Adjust the x position according to your needs
//        pokemonArtRect.y = 0;                                                       // Adjust the y position according to your needs
//        pokemonArtRect.w = pokeListArtSurface->w * 1.2;                                    // Adjust the width to fill the space to the left of the list
//        pokemonArtRect.h = pokeListArtSurface->h * 1.2;                                    // Adjust the height to fill the window height
//
//        // Blits pokemon image pixel art surface to pokemonListEntrySurface
//        SDL_FreeSurface(pokeListArtSurface);
//        pokeListArtSurface = optimizedSurface;
//        SDL_BlitScaled(pokeListArtSurface, NULL, *pokemonListEntrySurface, &pokemonArtRect);
//        SDL_FreeSurface(pokeListArtSurface);
//
//        ////------------------------------------------------------------////////////
//            // Create pokemon type pixel art surface
//        pokeListType1Surface = IMG_Load(typePath1.c_str());
//        if (pokeListType1Surface == NULL) {
//            cout << "Unable to IMG_Load! SDL Error: " << IMG_GetError() << endl;
//            exit;
//        }
//        optimizedSurface = SDL_ConvertSurfaceFormat(pokeListType1Surface, SDL_PIXELFORMAT_RGBA32, 0);
//        if (optimizedSurface == NULL) {
//            cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << endl;
//        }
//
//        // Define a rect for the surface
//        SDL_Rect pokemonType1Rect;
//        pokemonType1Rect.x = pokemonArtRect.x + pokemonArtRect.w; // Same x position as the Pokémon image
//        pokemonType1Rect.y = ((*pokemonListEntrySurface)->h - pokeListType1Surface->h * 2) / 2; // Adjust for two type images
//        pokemonType1Rect.w = pokeListType1Surface->w * 2; // Use the width of the type image
//        pokemonType1Rect.h = pokeListType1Surface->h * 2; // Use the height of the type image
//
//        SDL_FreeSurface(pokeListType1Surface);
//        pokeListType1Surface = optimizedSurface;
//        SDL_BlitScaled(pokeListType1Surface, NULL, *pokemonListEntrySurface, &pokemonType1Rect);
//        SDL_FreeSurface(pokeListType1Surface);
//
//        ////------------------------------------------------------------////////////
//        // Check if pokemon has second type, and render it to screen. 
//        if (pokemon[3] != "") {
//            typePath2 = "resources/types/" + pokemon[3] + ".png"; // Assuming the 4th column is the primary type
//            cout << "typePath2: " << typePath2 << endl;
//            pokeListType2Surface = IMG_Load(typePath2.c_str());
//            if (pokeListType2Surface == NULL) {
//                cout << "IMG_Load: " << IMG_GetError() << endl;
//                exit;
//            }
//            optimizedSurface = SDL_ConvertSurfaceFormat(pokeListType2Surface, SDL_PIXELFORMAT_RGBA32, 0);
//            if (optimizedSurface == NULL) {
//                cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << endl;
//            }
//
//            SDL_Rect pokemonType2Rect;
//            pokemonType2Rect.x = pokemonArtRect.x + pokemonArtRect.w; // Same x position as the Pokémon image
//            pokemonType2Rect.y = pokemonType1Rect.y + pokemonType1Rect.h; // Position it below the first type image
//            pokemonType2Rect.w = pokeListType2Surface->w * 2; // Use the width of the type image
//            pokemonType2Rect.h = pokeListType2Surface->h * 2; // Use the height of the type image
//
//            SDL_FreeSurface(pokeListType2Surface);
//            pokeListType2Surface = optimizedSurface;
//            SDL_BlitScaled(pokeListType2Surface, NULL, *pokemonListEntrySurface, &pokemonType2Rect);
//            SDL_FreeSurface(pokeListType2Surface);
//
//            // Add a delay
//            //SDL_Delay(2000);  // Wait for 100 milliseconds
//        }
//
//    }
//
//    void pokemonMinimalStats(SDL_Surface * *pokemonListEntrySurface, TTF_Font * *font, int& selectedItem, int& maxListTextWidth) {
//        // Get all * from pokemon Table
//        const string SQL_STRING_GET_POKEMONS =
//            "SELECT p.id, p.identifier, "
//            "MAX(CASE WHEN ps.stat_id = 1 THEN ps.base_stat END) AS hp, "
//            "MAX(CASE WHEN ps.stat_id = 2 THEN ps.base_stat END) AS attack, "
//            "MAX(CASE WHEN ps.stat_id = 3 THEN ps.base_stat END) AS defense, "
//            "MAX(CASE WHEN ps.stat_id = 4 THEN ps.base_stat END) AS special_attack, "
//            "MAX(CASE WHEN ps.stat_id = 5 THEN ps.base_stat END) AS special_defense, "
//            "MAX(CASE WHEN ps.stat_id = 6 THEN ps.base_stat END) AS speed "
//            "FROM pokemon p "
//            "JOIN pokemon_stats ps ON p.id = ps.pokemon_id "
//            "WHERE p.id <= 649 "
//            "GROUP BY p.id, p.identifier "
//            "ORDER BY p.id";
//
//        //cout << "executeSQL(SQL_STRING_GET_POKEMONS) \n";
//        vector<vector<string>> results = executeSQL(SQL_STRING_GET_POKEMONS);
//
//        // Get the selected Pokémon's stats from the results vector
//        vector<string> pokemon = results[selectedItem];
//        SDL_Color color = { 255, 255, 255 };
//
//
//        size_t underscorePos = pokemon[1].find('-');
//        pokemon[1][0] = toupper(pokemon[1][0]);
//        // If an underscore is found, get the substring up to the underscore
//        string pokemonName = underscorePos != string::npos ? pokemon[1].substr(0, underscorePos) : pokemon[1];
//
//        // Create a string stream to build the stats text
//        stringstream statsStream;
//
//        // Add the Pokémon's stats to the stream
//        pokemon[1][0] = toupper(pokemon[1][0]);
//        //statsStream << left << setw(5) << pokemonName << " ";
//        statsStream << right << setw(7) << pokemon[2] << " ";
//        statsStream << right << setw(5) << pokemon[3] << " ";
//        statsStream << right << setw(5) << pokemon[4] << " ";
//        statsStream << right << setw(5) << pokemon[5] << " ";
//        statsStream << right << setw(5) << pokemon[6] << " ";
//        statsStream << right << setw(5) << pokemon[7] << " ";
//        statsStream << right << setw(5) << (pokemon[2] + pokemon[3] + pokemon[4] + pokemon[5] + pokemon[6] + pokemon[7]) << "";
//
//        // Convert the stream to a string
//        string statsText = statsStream.str();
//
//        // Render the text into an SDL_Surface
//        SDL_Surface* pokeListStatsSurface = NULL;
//
//        //vector<string> lines = splitLines(statsText);
//        //int lineHeight = 35; // Adjust this to match your font's height
//        //int padding = 50;
//        //for (int i = 0; i < lines.size(); i++) {
//        pokeListStatsSurface = TTF_RenderText_Blended(
//            *font,
//            statsText.c_str(),
//            color
//        );
//        if (font == NULL) {
//            cout << "TTF_OpenFont: " << TTF_GetError() << endl;
//            // handle error...
//        }
//
//        SDL_Rect pokemonStatsRect;
//        pokemonStatsRect.x = ((*pokemonListEntrySurface)->w - pokeListStatsSurface->w * 2); // Adjust for two type images
//        pokemonStatsRect.y = ((*pokemonListEntrySurface)->h - pokeListStatsSurface->h * 2); // Adjust for two type images
//        pokemonStatsRect.w = pokeListStatsSurface->w;
//        pokemonStatsRect.h = pokeListStatsSurface->h;
//        // Clean up pokemonArtRect pokeListArtSurface
//        SDL_BlitSurface(pokeListStatsSurface, NULL, *pokemonListEntrySurface, &pokemonStatsRect);
//        SDL_FreeSurface(pokeListStatsSurface);
//        // }
//
//    }
//
//    void PokeDexFullList(SDL_Surface * *screen, SDL_Renderer * *renderer, TTF_Font * *font, int& topItem, int& selectedItem, int& maxListTextWidth) {
//        // Get all * from pokemon Table
//        string SQL_STRING_GET_POKEMONS =
//            "SELECT * FROM pokemon "
//            "LIMIT 649;";
//        //cout << "executeSQL(SQL_STRING_GET_POKEMONS) \n";
//        vector<vector<string>> results = executeSQL(SQL_STRING_GET_POKEMONS);
//        int pokemonListSurfaceHeight = static_cast<int>(WINDOW_HEIGHT / 5);
//
//        // Iterate over all Pokémon in the results
//        for (int i = 0; i < results.size() - 1; i++) {
//            /*/int counter = 0;
//            for (const auto& row : results) {     // for each row in results
//                //if (counter >= 648) break;
//                for (const auto& col : row) {     // for each column in a row
//                    cout << col;                  // do something..
//                };
//                printf("\n");
//                //counter++;
//            };*/
//
//            SDL_Color rainbowColors[] = {
//                {119, 221, 119},   // Pastel Green
//                //{95, 177, 95},     // Darker shade 1 of Pastel Green
//                //{71, 133, 71},     // Darker shade 2 of Pastel Green
//                //{48, 88, 48},      // Darker shade 3 of Pastel Green
//                {255, 105, 97},    // Pastel Red
//                //{204, 85, 78},     // Darker shade 1 of Pastel Red
//                //{153, 64, 59},     // Darker shade 2 of Pastel Red
//                //{102, 43, 39},     // Darker shade 3 of Pastel Red
//                {119, 158, 203},   // Pastel Blue
//                //{95, 126, 162},    // Darker shade 1 of Pastel Blue
//                //{71, 95, 122},     // Darker shade 2 of Pastel Blue
//                //{48, 63, 81},      // Darker shade 3 of Pastel Blue		
//                {102, 102, 153},   // Pastel Indigo
//                //{82, 82, 122},     // Darker shade 1 of Pastel Indigo
//                //{61, 61, 92},      // Darker shade 2 of Pastel Indigo
//                {41, 41, 61},      // Darker shade 3 of Pastel Indigo
//                //{221, 160, 221},   // Pastel Violet
//                //{177, 128, 177},   // Darker shade 1 of Pastel Violet
//                //{133, 96, 133},    // Darker shade 2 of Pastel Violet
//                //{88, 64, 88},      // Darker shade 3 of Pastel Violet
//                {255, 179, 71},    // Pastel Orange
//                //{204, 143, 57},    // Darker shade 1 of Pastel Orange
//                //{153, 107, 43},    // Darker shade 2 of Pastel Orange
//                //{102, 71, 28},     // Darker shade 3 of Pastel Orange
//                {253, 253, 150},   // Pastel Yellow
//                //{202, 202, 120},   // Darker shade 1 of Pastel Yellow
//                //{152, 152, 90},    // Darker shade 2 of Pastel Yellow
//                //{101, 101, 60},    // Darker shade 3 of Pastel Yellow
//            };
//
//            // Get the number of colors
//            int numColors = sizeof(rainbowColors) / sizeof(SDL_Color);
//
//
//
//
//            SDL_Surface* pokemonListEntrySurface = NULL,
//                * textListSurface = NULL,
//                * pokeListIDSurface = NULL;
//            ostringstream textStream;
//            SDL_Color color = { 255, 255, 255 };
//            SDL_Color highlightColor = { 255, 0, 0 };
//            vector<string> pokemon = results[i];
//
//            //print current pokemon 
//            for (int i = 0; i < pokemon[i].size(); i++) {
//                // Print the contents of the vector
//                for (const string& value : pokemon) {
//                    cout << value << " ";
//                }
//                cout << endl;  // Add a newline between each Pokemon entry
//            }
//
//
//            // Use modulo to cycle through the colors
//
//            SDL_Color background_color = rainbowColors[i % numColors];
//            pokemonListEntrySurface = SDL_CreateRGBSurface(
//                0,
//                WINDOW_WIDTH,
//                pokemonListSurfaceHeight,
//                DEPTH,
//                0, 0, 0, 0
//            );
//            SDL_FillRect(
//                pokemonListEntrySurface,
//                NULL,
//                SDL_MapRGBA(
//                    pokemonListEntrySurface->format,
//                    background_color.r,
//                    background_color.g,
//                    background_color.b,
//                    128
//                )
//            );
//
//            int labelHeight = pokemonListEntrySurface->h;
//            SDL_Rect pokeListEntry;
//            pokeListEntry.x = 0; // A small margin from the left
//            pokeListEntry.y = i * pokemonListSurfaceHeight;//(i - selectedItem) * 24;
//            pokeListEntry.w = pokemonListEntrySurface->w;
//            pokeListEntry.h = pokemonListEntrySurface->h;
//
//
//            // TODO: Make get pokemonID()
//            bool isSelected = (i == selectedItem);
//            pokemonID(pokemon, &pokemonListEntrySurface, font, isSelected, maxListTextWidth);
//
//            // TODO: Make get pokemonPixelArt()
//            pokemonListPixelArt(&pokemonListEntrySurface, i, selectedItem, maxListTextWidth);
//
//            // TODO: Make get pokemonMinimalStats()
//            pokemonMinimalStats(&pokemonListEntrySurface, font, selectedItem, maxListTextWidth);
//
//
//            SDL_BlitSurface(pokemonListEntrySurface, NULL, *screen, &pokeListEntry);
//            SDL_FreeSurface(pokemonListEntrySurface);
//            SDL_FreeSurface(pokeListIDSurface);
//
//
//            // Generate Pokemon Image
//            // call PokemonPixelArt function to generate Pokemon Image
//            //PokemonPixelArt(renderer, screen, texture, selectedItem, maxListTextWidth);
//
//            // Generate Full Pokemon List
//            //call PokemonFullList function to generate full list of pokemon    
//            //PokemonFullList(screen, font, selectedItem, maxListTextWidth);
//        }
//    }
//
//    int main(int argc, char* argv[])
//    {
//        // Vriable for SDL components 
//        SDL_Window* window = NULL;
//        SDL_Renderer* renderer = NULL;
//        SDL_Texture* texture = NULL;
//        SDL_Surface* screen = NULL;
//        SDL_Color color = { 255, 255, 255 };
//
//        //initizlize SDL components
//        InitializeSDL(&window, &renderer, &texture, &screen);
//
//        // Check Access to DB
//        const string SQL_STRING_GET_POKEMONS_COUNT = "SELECT COUNT(*) FROM pokemon";//"SELECT COUNT(*) FROM pokemon"
//        sqlite3* db;
//        int rc;
//        char* zErrMsg = 0;
//        // Open DP for Stats
//        rc = sqlite3_open("pokedex.sqlite", &db);
//        if (rc != SQLITE_OK) {
//            cout << "SQLite coult not open file! SQLite Error: " << sqlite3_errmsg(db) << endl;
//            sqlite3_close(db);
//            return EXIT_FAILURE;
//        }
//        else {
//            cout << "SQLite Opened file! " << endl;
//            sqlite3_close(db);
//        }
//
//        cout << "executeSQL(SQL_STRING_GET_POKEMONS_COUNT) \n";
//        vector<vector<string>> sql = executeSQL(SQL_STRING_GET_POKEMONS_COUNT);
//        for (const auto& row : sql) {         // for each row in results
//            for (const auto& col : row) {     // for each column in a row
//                cout << col;                  // do something..
//            };
//            printf("\n");
//        };
//
//        // Variables to handle navigation
//        int totalItems = stoi(sql[0][0]);
//        cout << totalItems << endl;
//
//        // Prepare Text Msg for displaying on screen
//        // Load a font
//        TTF_Font* font = TTF_OpenFont("Pokemon_GB.ttf", 24);
//        if (font == NULL) {
//            cout << "TTF_OpenFont: " << TTF_GetError() << endl;
//            return EXIT_FAILURE;
//        }
//
//        // Variables to handle navigation      
//        int topItem = 0; // The index of the top item on the screen
//        int selectedItem = 0;  // The index of the selected item on the screen
//        int maxListTextWidth = 0;
//        bool quit = false;
//
//        const Uint8* state = SDL_GetKeyboardState(NULL);
//        View currentView = View::LIST;
//        SDL_Event windowEvent;
//        while (!quit) {
//
//            while (SDL_PollEvent(&windowEvent))
//            {
//                if (windowEvent.type == SDL_QUIT) {
//                    quit = true;
//                }
//                else if (windowEvent.type == SDL_KEYDOWN) {
//                    switch (windowEvent.key.keysym.sym) {
//                    case SDLK_RIGHT:
//                        currentView = View::STATS;
//                        break;
//                    case SDLK_LEFT:
//                        currentView = View::LIST;
//                        break;
//                    case SDLK_UP:
//                        if (selectedItem > 0) {
//                            selectedItem--;
//                        }
//                        break;
//                    case SDLK_DOWN:
//                        if (selectedItem < totalItems - 1) {
//                            selectedItem++;
//                        }
//                        break;
//                    case SDLK_ESCAPE: // Add this case for the ESC key
//                        quit = true;
//                        break;
//                    default:
//                        break;
//                    }
//                }
//            }
//            SDL_RenderClear(renderer);
//            if (currentView == View::LIST) {
//                // Clear the window and make it all white
//
//                // TODO:  Replace this section with 1 function - PokedexFullList
//                // PokeDexFullList generates all the rows for every pokemon. 
//                PokeDexFullList(&screen, &renderer, &font, topItem, selectedItem, maxListTextWidth);
//            }
//            else if (currentView == View::STATS) {
//                // Clear the window and make it all white
//                SDL_RenderClear(renderer);
//
//                // Generate Stats for selected Pokemon
//                // call PokemonStatsView function to generate Stats for selected Pokemon 
//                //PokemonStatsView(screen, font, selectedItem, maxListTextWidth);
//            }
//            // Flip the backbuffer
//            SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
//            SDL_RenderClear(renderer);
//            SDL_RenderCopy(renderer, texture, NULL, NULL);
//            SDL_RenderPresent(renderer);
//        }
//        // Clean up
//        SDL_FreeSurface(screen);
//        TTF_CloseFont(font);
//        TTF_Quit();
//        SDL_DestroyTexture(texture);
//        SDL_DestroyRenderer(renderer);
//        SDL_DestroyWindow(window);
//        sqlite3_close(db);
//        SDL_Quit();
//        return EXIT_SUCCESS;
//    };