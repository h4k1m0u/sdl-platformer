#ifndef TYPES_HPP
#define TYPES_HPP

#include <unordered_map>
#include <vector>
#include <array>
#include <SDL2/SDL_rect.h>

#include "tile_type.hpp"
#include "direction.hpp"

using Tiles = std::unordered_map<TILE_TYPE, std::vector<SDL_Point>>;
using PatrolTrajectory = std::pair<SDL_Point, SDL_Point>;

template <int N_FRAMES>
using Frames = std::array<SDL_Point, N_FRAMES>;

template <int N_FRAMES>
using Clips = std::unordered_map<Direction, Frames<N_FRAMES>>;

using BboxesMap = std::unordered_map<int, SDL_Rect>;

#endif
