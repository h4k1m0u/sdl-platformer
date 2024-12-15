#ifndef TYPES_HPP
#define TYPES_HPP

#include <unordered_map>
#include <vector>
#include <SDL2/SDL_rect.h>

#include "tile_type.hpp"

using Tiles = std::unordered_map<TILE_TYPE, std::vector<SDL_Point>>;

#endif
