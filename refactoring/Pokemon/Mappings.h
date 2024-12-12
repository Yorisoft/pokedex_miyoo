#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <map>
#include <string>
#include <vector>

// Define and initialize the global constant map
extern const std::map<std::string, std::vector<std::vector<int>>> color_map;
extern const std::map<int, std::vector<int>> gameColorMap;
extern const std::map<std::string, int> languageMap;
extern const std::map<std::string, int> regionMap;
#endif // COLORMAP_H
