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
#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>                       // required for SDL operations
#include <SDL2/SDL_image.h>                   // required for SDL text rendering operations
#include <SDL2/SDL_ttf.h>
#include "sqlite/sqlite3.h"
using namespace std;
using json = nlohmann::json;

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
                    if (selectedItem < results.size() - 1) selectedItem++;
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