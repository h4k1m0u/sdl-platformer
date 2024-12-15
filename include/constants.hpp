#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SDL2/SDL_pixels.h>
#include <unordered_set>

#include "tile_type.hpp"

namespace Constants {
  const int WIDTH_TILE = 32;
  const int HEIGHT_TILE = 32;

  /* 24 tiles for width/height (tile=32px) */
  const int SCREEN_WIDTH = 768;
  const int SCREEN_HEIGHT = 768;

  /* four screens (quadrants) by level */
  const int LEVEL_WIDTH = SCREEN_WIDTH * 2;
  const int LEVEL_HEIGHT = SCREEN_HEIGHT * 2;

  const bool DEBUG = true;
  const SDL_Color COLOR_BBOX = { 0xff, 0, 0, 0xff };

  const std::unordered_set<TILE_TYPE> GROUNDS = { TILE_TYPE::ROCK, TILE_TYPE::GRASS, TILE_TYPE::STONE };
}

#endif
