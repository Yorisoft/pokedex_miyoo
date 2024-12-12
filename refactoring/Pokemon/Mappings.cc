#include"Mappings.h"

// Define and initialize the global constant map
const std::map<std::string, std::vector<std::vector<int>>> color_map = {
    {"bug", {{103, 116, 69}, {81, 91, 52}}},                // Ebony
    {"dark", {{55, 55, 65}, {40, 40, 50}}},                 // DimGray and DarkGray
    {"dragon", {{76, 63, 105}, {59, 50, 81}}},              // SteelBlue and CornflowerBlue
    {"electric", {{90, 90, 30}, {70, 70, 20}}},             // Field drab, Dark drab brown
    {"fairy", {{121, 67, 103}, {100, 48, 74}}},             // Pink and LightPink
    {"fighting", {{78, 45, 45}, {57, 34, 34}}},             // Orange and Gold
    {"fire", {{111, 53, 17}, {86, 42, 19}}},                // Russet, Seal brown
    {"flying", {{76, 91, 87}, {54, 71, 68}}},               // Feldgrau, Dark slate grey
    {"ghost", {{85, 70, 93}, {66, 51, 74}}},                // SlateBlue and MediumSlateBlue
    {"grass", {{44, 61, 43}, {40, 52, 39}}},                // Dark green, Black Olive
    {"ground", {{89, 69, 52}, {71, 59, 46}}},               // Walnut brown, Taupe
    {"ice", {{73, 84, 91}, {55, 60, 64}}},                  // Onyx, Gunmetal
    {"normal", {{105, 112, 95}, {76, 83, 66}}},             // BlanchedAlmond and Cornsilk
    {"poison", {{92, 77, 89}, {67, 54, 71}}},               // MediumPurple and Purple
    {"psychic", {{91, 63, 79}, {76, 45, 63}}},              // Plum and Violet
    {"rock", {{71, 67, 63}, {61, 58, 55}}},                 // Variants of Rock colors
    {"steel", {{99, 117, 125}, {79, 94, 100}}},             // Silver and LightGray
    {"water", {{43, 58, 78}, {33, 46, 62}}}                 // Charcoal, Gunmetal
};
// Define and initialize the global constant map
const std::map<int, std::vector<int>> gameColorMap = {
        {1, {111, 53, 17}},    // Red - Fire-like color, Russet
        {2, {43, 58, 78}},     // Blue - Water-like color, Charcoal
        {3, {90, 90, 30}},     // Yellow - Electric-like color, Field drab
        {4, {78, 45, 45}},     // Gold - Fighting theme color, Orange
        {5, {99, 117, 125}},   // Silver - Steel-like color, Silver
        {6, {73, 84, 91}},     // Crystal - Onyx-like color, Onyx
        {7, {111, 53, 17}},    // Ruby - Fire-like color variant (same as red)
        {8, {43, 58, 78}},     // Sapphire - Water-like color variant (same as blue)
        {9, {44, 61, 43}},     // Emerald - Grass-like color
        {10, {111, 53, 18}},   // FireRed - Fire-like color variant
        {11, {44, 62, 43}},    // LeafGreen - Grass-like color variant
        {12, {71, 67, 63}},    // Diamond - Rock color
        {13, {121, 67, 103}},  // Pearl - Fairy-like color
        {14, {98, 116, 124}},  // Platinum - Steel color variant
        {15, {77, 46, 46}},    // HeartGold - Fighting theme color variant
        {16, {98, 116, 124}},  // SoulSilver - Steel color variant
        {17, {55, 55, 65}},    // Black - Dark theme color, DimGray
        {18, {105, 112, 95}},  // White - Normal theme color, BlanchedAlmond
        {19, {50, 50, 60}},    // Black 2 - Dark theme variant
        {20, {100, 107, 90}},  // White 2 - Normal theme variant
        {21, {91, 63, 79}},    // X - Psychic theme color
        {22, {88, 60, 76}},    // Y - Psychic theme variant
        {23, {114, 55, 19}},   // Omega Ruby - Fire-like color variant
        {24, {46, 61, 81}},    // Alpha Sapphire - Water-like color variant
        {25, {113, 55, 19}},   // Sun - Fire-like color variant
        {26, {85, 70, 93}},    // Moon - Ghost theme color
        {27, {116, 58, 22}},   // Ultra Sun - Fire-like color variant
        {28, {88, 73, 96}},    // Ultra Moon - Ghost theme variant
        {29, {99, 117, 125}},  // Sword - Steel theme color
        {30, {96, 114, 122}},  // Shield - Steel theme variant
        {31, {71, 67, 63}},    // Brilliant Diamond - Rock color
        {32, {118, 65, 100}},  // Shining Pearl - Fairy-like color variant
        {33, {103, 116, 69}},  // Legends: Arceus - Bug theme color
        {34, {111, 53, 17}},   // Scarlet - Fire-like color (same as red)
        {35, {92, 77, 89}}     // Violet - Poison theme color
};
const std::map<std::string, int> languageMap = {
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

const std::map<std::string, int> regionMap = {
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
