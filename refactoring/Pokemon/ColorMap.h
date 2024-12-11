#ifndef COLORMAP_H
#define COLORMAP_H

#include <map>
#include <string>
#include <vector>

// Define and initialize the global constant map
extern const std::map<std::string, std::vector<std::vector<int>>> color_map = {
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
#endif // COLORMAP_H
