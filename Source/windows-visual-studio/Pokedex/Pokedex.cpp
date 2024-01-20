// Pokedex.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WINDOW_HEIGHT 480                   // window height in pixels
#define WINDOW_WIDTH 640                    // window width in pixels
#define POKEMON_LIST_SIZE 649               // Total pokemon to load ( up to gen 5 )
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream> 
#include <SDL.h>                            // required for SDL operations
#include <SDL_ttf.h>                        // required for SDL text rendering operations
#include <SDL_image.h>
#include "sqlite/sqlite3.h"
using namespace std;

vector<vector<string>> results;             // Declare results vector outside of main
static int callback(void* data, int argc, char** argv, char** azColName) {
    results.emplace_back(argv, argv + argc); // Use the global results vector
    return 0;
}
vector<string> splitLines(const string& text) {
    vector<string> lines;
    stringstream ss(text);
    string line;
    while (getline(ss, line, '\n')) {
        lines.push_back(line);
    }
    return lines;
}
enum class View {
    LIST,
    STATS
};
int main(int argc, char* argv[])
{
    char* zErrMsg = 0;
    SDL_Surface* textListSurface = NULL;
    SDL_Texture* textListTexture = NULL;
    SDL_Renderer* renderer = NULL;
    const char* SQL_STRING_GET_POKEMONS = "SELECT * FROM pokemon LIMIT 649";

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
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    // Create a 2d Rendering context for the window
    renderer = SDL_CreateRenderer(window, -1, 0);
    //background color: (r, g, b, alpha)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Prepare Text Msg for displaying on screen
    // Load a font
    TTF_Font* font = TTF_OpenFont("Pokemon_GB.ttf", 20);
    if (font == NULL) {
        cout << "TTF_OpenFont: " << TTF_GetError() << endl;
        return EXIT_FAILURE;
    }
    // Set the color of the text
    SDL_Color color = { 255, 255, 255 };
    // Color to highlight the selected item
    SDL_Color highlightColor = { 255, 0, 0 }; 

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
                    if (selectedItem >= topItem + WINDOW_HEIGHT / 24) topItem++; // Adjust the 30 according to your font height
                    break;
                default:
                    break;
                }
            }
        }

        // Clear the window and make it all white
        SDL_RenderClear(renderer);
        if (currentView == View::LIST) {
            // Render each Pokemon name
            for (int i = selectedItem; i < results.size() && i < selectedItem + WINDOW_HEIGHT / 24; i++) { // Adjust the 30 according to your font height
                string labelText = "ID No. Name";

                
                ostringstream textStream;

                results[i][1][0] = toupper(results[i][1][0]);                
                size_t underscorePos = results[i][1].find('-');
    
				// If an underscore is found, get the substring up to the underscore
				string pokemonName = underscorePos != string::npos ? results[i][1].substr(0, underscorePos) : results[i][1];
				
				textStream << right << setw(4) << results[i][0] << " " << pokemonName; // setw(4) assumes Pokemon ID is up to 3 digits.

                string text = textStream.str();

                // Render the text into an SDL_Surface
                SDL_Surface* labelSurface = TTF_RenderText_Blended(
                    font, 
                    labelText.c_str(), 
                    color
                );
                if (labelSurface == NULL) {
                    cout << "TTF_RenderText_Blended Error: " << TTF_GetError() << endl;
                    return EXIT_FAILURE;
                };
                // Render the text into an SDL_Surface
                textListSurface = TTF_RenderText_Blended(
                    font,
                    text.c_str(),
                    i == selectedItem ? highlightColor : color
                );


                // Convert the surface to a texture
                SDL_Texture* labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface);
                textListTexture = SDL_CreateTextureFromSurface(renderer, textListSurface);


                
                if (textListSurface->w > maxListTextWidth) {
                    maxListTextWidth = textListSurface->w;
                }
                // Define a rect for the texture
                SDL_Rect textRect;
                int labelHeight = labelSurface->h;
                textRect.x = (WINDOW_WIDTH - maxListTextWidth) - 10; // A small margin from the left
                textRect.y = (i - selectedItem) * 24 + labelHeight;  // Adjust the y position for each item
                textRect.w = textListSurface->w;
                textRect.h = textListSurface->h;

                SDL_Rect labelRect;
                labelRect.x = (WINDOW_WIDTH - maxListTextWidth) - 10; // Same x position as the list
                labelRect.y = 0; // Position it at the top
                labelRect.w = labelSurface->w;
                labelRect.h = labelSurface->h;

                // Copy the texture to the rendering context
                SDL_RenderCopy(renderer, textListTexture, NULL, &textRect);
                SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);

                // Clean up
                SDL_FreeSurface(textListSurface);
                SDL_DestroyTexture(textListTexture);
                SDL_FreeSurface(labelSurface);
                SDL_DestroyTexture(labelTexture);

            }
        }
        else if (currentView == View::STATS) {
            // Get the selected Pokémon's stats from the results vector
            vector<string> selectedPokemon = results[selectedItem];

            // Create a string stream to build the stats text
            stringstream statsStream;


            // Add the Pokémon's stats to the stream
            statsStream << "Name: " << selectedPokemon[1] << "\n";
            statsStream << "HP: " << selectedPokemon[6] << "\n";
            statsStream << "Attack: " << selectedPokemon[7] << "\n";
            statsStream << "Defense: " << selectedPokemon[8] << "\n";
            statsStream << "Sp. Atk: " << selectedPokemon[9] << "\n";
            statsStream << "Sp. Def: " << selectedPokemon[10] << "\n";
            statsStream << "Speed: " << selectedPokemon[11] << "\n";
            statsStream << "Total: " << selectedPokemon[5] << "\n";

            // Convert the stream to a string
            string statsText = statsStream.str();

            // Render the text into an SDL_Surface
            SDL_Surface* statsSurface = TTF_RenderText_Blended(font, statsText.c_str(), color);

            // Convert the surface to a texture
            SDL_Texture* statsTexture = SDL_CreateTextureFromSurface(renderer, statsSurface);

            // Define a rect for the texture
            SDL_Rect statsRect;
            statsRect.x = (WINDOW_WIDTH - maxListTextWidth) - 50; // Adjust the x position according to your needs
            statsRect.y = (WINDOW_HEIGHT / 4); // Adjust the y position according to your needs
            statsRect.w = statsSurface->w;
            statsRect.h = statsSurface->h;

            vector<string> lines = splitLines(statsText);
            int lineHeight = 20; // Adjust this to match your font's height
            for (int i = 0; i < lines.size(); i++) {
                SDL_Surface* lineSurface = TTF_RenderText_Blended(font, lines[i].c_str(), color);
                SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(renderer, lineSurface);
                SDL_Rect lineRect;
                lineRect.x = statsRect.x;
                lineRect.y = statsRect.y + i * lineHeight;
                lineRect.w = lineSurface->w;
                lineRect.h = lineSurface->h;
                SDL_RenderCopy(renderer, lineTexture, NULL, &lineRect);
                SDL_FreeSurface(lineSurface);
                SDL_DestroyTexture(lineTexture);
            }

            // Clean up
            SDL_FreeSurface(statsSurface);
            SDL_DestroyTexture(statsTexture);
        }
        string imagePath = "resources/full_pic/" + results[selectedItem][1] + ".png";
        string typePath1 = "resources/types/" + results[selectedItem][3] + ".png"; // Assuming the 4th column is the primary type
        string typePath2;
        
        SDL_Surface* pokemonImage = IMG_Load(imagePath.c_str());
        if (pokemonImage == NULL) {
            cout << "IMG_Load: " << IMG_GetError() << endl;
            return EXIT_FAILURE;
        }
        SDL_Surface* typeImage1 = IMG_Load(typePath1.c_str());
        if (typeImage1 == NULL) {
            cout << "IMG_Load: " << IMG_GetError() << endl;
            return EXIT_FAILURE;
        }
        float aspectRatio = (float)typeImage1->w / typeImage1->h;
        int typeRectHeight = (WINDOW_WIDTH - maxListTextWidth - 100) / aspectRatio; // Adjust the width to match the Pokémon image
        // Convert the surface to a texture
        SDL_Texture* pokemonTexture = SDL_CreateTextureFromSurface(renderer, pokemonImage);
        SDL_Texture* typeTexture1 = SDL_CreateTextureFromSurface(renderer, typeImage1);  

        // Define a rect for the texture
        SDL_Rect pokemonRect;
        pokemonRect.x = 0; // Adjust the x position according to your needs
        pokemonRect.y = 0; // Adjust the y position according to your needs
        pokemonRect.w = (WINDOW_WIDTH - maxListTextWidth) - 150; // Adjust the width to fill the space to the left of the list
        pokemonRect.h = pokemonRect.w; // Adjust the height to fill the window height

        SDL_Rect typeRect1;
        typeRect1.x = 0; // Same x position as the Pokémon image
        typeRect1.y = pokemonRect.h; // Position it below the Pokémon image
        typeRect1.w = typeImage1->w * 2; // Use the width of the type image
        typeRect1.h = typeImage1->h * 2; // Use the height of the type image

        // Copy the texture to the rendering context
        SDL_RenderCopy(renderer, pokemonTexture, NULL, &pokemonRect);
        SDL_RenderCopy(renderer, typeTexture1, NULL, &typeRect1);

        // Check if pokemon has second type, and render it to screen. 
        SDL_Surface* typeImage2 = NULL;
        SDL_Texture* typeTexture2 = NULL;
        if (results[selectedItem][4] != " ") {
            typePath2 = "resources/types/" + results[selectedItem][4] + ".png"; // Assuming the 4th column is the primary type

            typeImage2 = IMG_Load(typePath2.c_str());
            if (typeImage2 == NULL) {
                cout << "IMG_Load: " << IMG_GetError() << endl;
                return EXIT_FAILURE;
            }

            typeTexture2 = SDL_CreateTextureFromSurface(renderer, typeImage2);

            SDL_Rect typeRect2;
            typeRect2.x = typeRect1.x + typeRect1.w; // Same x position as the Pokémon image
            typeRect2.y = pokemonRect.h; // Position it below the first type image
            typeRect2.w = typeImage1->w * 2; // Use the width of the type image
            typeRect2.h = typeImage1->h * 2; // Use the height of the type image

            SDL_RenderCopy(renderer, typeTexture2, NULL, &typeRect2);
        }      

        SDL_FreeSurface(pokemonImage);
        SDL_DestroyTexture(pokemonTexture);
        SDL_FreeSurface(typeImage1);
        SDL_DestroyTexture(typeTexture1);
        SDL_FreeSurface(typeImage2);
        SDL_DestroyTexture(typeTexture2);

        // Flip the backbuffer
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_FreeSurface(textListSurface);
    SDL_DestroyTexture(textListTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    textListSurface = NULL;
    textListTexture = NULL;
    renderer = NULL;

    SDL_DestroyWindow(window);
    sqlite3_close(db);
    SDL_Quit();
    return EXIT_SUCCESS;
}

