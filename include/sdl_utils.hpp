#ifndef SDL_UTILS_HPP
#define SDL_UTILS_HPP

#include <SDL2/SDL_rect.h>

namespace SDLUtils {
  bool operator==(const SDL_Point& point1, const SDL_Point& point2);
}

#endif
