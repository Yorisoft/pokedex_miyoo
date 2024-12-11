#include "PokedexActivityMenu.h"
#include "PokedexActivityManager.h"

PokedexActivityMenu PokedexActivityMenu::instance;

PokedexActivityMenu::PokedexActivityMenu() {
    surf_logo = NULL;
    dbResults = NULL;
    languageMap = {
        {"ja-Hrkt", 1},
        {"roomaji", 2},
        {"ko", 3},
        {"zh-Hant", 4},
        {"fr", 5},
        {"de", 6},
        {"es", 7},
        {"it", 8},
        {"en", 9},
        {"cs", 10},
        {"ja", 11},
        {"zh-Hans", 12},
        {"pt-BR", 13}
    };
    regionMap = {
        {"red", 1},  // Kanto
        {"blue", 1}, // Kanto
        {"yellow", 1}, // Kanto
        {"gold", 3},  // Johto (original)
        {"silver", 3},  // Johto (original)
        {"crystal", 3},  // Johto (original)
        {"ruby", 4},  // Hoenn
        {"sapphire", 4},  // Hoenn
        {"emerald", 4},  // Hoenn
        {"diamond", 5},  // Sinnoh (original)
        {"pearl", 5},  // Sinnoh (original)
        {"platinum", 5},  // Sinnoh (original)
        {"black", 8},  // Unova (original)
        {"white", 8},  // Unova (original)
        {"black2", 8},  // Unova (updated)
        {"white2", 8},  // Unova (updated)
        {"x", 6},  // Kalos
        {"y", 6},  // Kalos
        {"omega-ruby", 4},  // Hoenn (remake)
        {"alpha-sapphire", 4},  // Hoenn (remake)
        {"sun", 7},  // Alola
        {"moon", 7},  // Alola
        {"ultra-sun", 7},  // Alola (updated)
        {"ultra-moon", 7},  // Alola (updated)
        {"sword", 8},  // Galar
        {"shield", 8},  // Galar
        {"brilliant-diamond", 5},  // Sinnoh (remake)
        {"shining-pearl", 5},  // Sinnoh (remake)
        {"legends-arceus", 5},  // Sinnoh (expanded)
        {"scarlet", 8},  // Paldea
        {"violet", 8}   // Paldea
    };
}

void PokedexActivityMenu::onActivate() {
    std::cout << "PokedexActivityMenu::onActivate START \n";
    int language_id = languageMap["es"];
    PokedexDB::setLanguageVersion(language_id);

    dbResults = PokedexDB::executeSQL(&SQL_getGameVersions);
    for (auto& game : *dbResults) {
        for (auto& col : game) {
            std::cout << col << " | ";
        }
        std::cout << std::endl;
    }

    std::string gameVersion;
    std::cin >> std::ws;
    std::getline(std::cin, gameVersion);
    PokedexDB::setGameVersion(gameVersion);

    int regionVersion = regionMap[gameVersion];
    PokedexDB::setRegionVersion(regionVersion);
    std::cout << "PokedexActivityMenu::onActivate END \n";
}

void PokedexActivityMenu::onDeactivate() {
    if (surf_logo) {
        SDL_FreeSurface(surf_logo);
        surf_logo = NULL;
    }
}

void PokedexActivityMenu::onLoop() {
    PokedexActivityManager::setActiveState(APPSTATE_POKEDEX_LIST);
}

void PokedexActivityMenu::onRender(SDL_Surface* surf_display, SDL_Renderer* renderer, SDL_Texture* texture) {
    // Clear the display surface
    //SDL_FillRect(surf_display, NULL, SDL_MapRGB(surf_display->format, 0, 0, 0));

    if (surf_logo) {
        int W = WINDOW_WIDTH;
        int H = WINDOW_HEIGHT;
        PokeSurface::onDraw(surf_display, surf_logo, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}

PokedexActivityMenu* PokedexActivityMenu::getInstance() {
    return &instance;
}