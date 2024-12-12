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
    {1, {205, 92, 92}},    // Red - Indian Red
    {2, {70, 130, 180}},   // Blue - Steel Blue
    {3, {218, 165, 32}},   // Yellow - Golden Rod
    {4, {210, 105, 30}},   // Gold - Chocolate
    {5, {192, 192, 192}},  // Silver - Silver
    {6, {112, 128, 144}},  // Crystal - Slate Gray
    {7, {178, 34, 34}},    // Ruby - Fire Brick
    {8, {65, 105, 225}},   // Sapphire - Royal Blue
    {9, {60, 179, 113}},   // Emerald - Medium Sea Green
    {10, {205, 92, 92}},   // FireRed - Indian Red (same as Red)
    {11, {85, 107, 47}},   // LeafGreen - Dark Olive Green
    {12, {169, 169, 169}}, // Diamond - Dark Gray
    {13, {255, 182, 193}}, // Pearl - Light Pink
    {14, {176, 196, 222}}, // Platinum - Light Steel Blue
    {15, {218, 165, 32}},  // HeartGold - Golden Rod (same as Yellow)
    {16, {192, 192, 192}}, // SoulSilver - Silver (same as Silver)
    {17, {47, 79, 79}},    // Black - Dark Slate Gray
    {18, {245, 245, 245}}, // White - White Smoke
    {19, {47, 79, 79}},    // Black 2 - Dark Slate Gray (same as Black)
    {20, {245, 245, 245}}, // White 2 - White Smoke (same as White)
    {21, {138, 43, 226}},  // X - Blue Violet
    {22, {147, 112, 219}}, // Y - Medium Purple
    {23, {178, 34, 34}},   // Omega Ruby - Fire Brick (same as Ruby)
    {24, {70, 130, 180}},  // Alpha Sapphire - Steel Blue (same as Blue)
    {25, {250, 128, 114}}, // Sun - Salmon
    {26, {176, 196, 222}}, // Moon - Light Steel Blue (same as Platinum)
    {27, {233, 150, 122}}, // Ultra Sun - Dark Salmon
    {28, {123, 104, 238}}, // Ultra Moon - Medium Slate Blue
    {29, {205, 133, 63}},  // Sword - Peru
    {30, {112, 128, 144}}, // Shield - Slate Gray (same as Crystal)
    {31, {160, 82, 45}},   // Brilliant Diamond - Sienna
    {32, {255, 160, 122}}, // Shining Pearl - Light Salmon
    {33, {144, 238, 144}}, // Legends: Arceus - Light Green
    {34, {240, 128, 128}}, // Scarlet - Light Coral
    {35, {186, 85, 211}}   // Violet - Medium Orchid
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
