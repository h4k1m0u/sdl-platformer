#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SDL2/SDL_pixels.h>

namespace Constants {
  /* four screens (quadrants) by level */
  const int LEVEL_WIDTH = 1280;
  const int LEVEL_HEIGHT = 960;

  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;

  const bool DEBUG = true;
  const SDL_Color COLOR_BBOX = { 0xff, 0, 0, 0xff };
}
#endif
