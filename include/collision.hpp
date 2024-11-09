#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <SDL2/SDL_rect.h>

namespace Collision {
  bool rect_to_rect(const SDL_Rect& rect1, const SDL_Rect& rect2);
};

#endif
