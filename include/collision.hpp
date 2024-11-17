#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <SDL2/SDL_rect.h>
#include <vector>

namespace Collision {
  enum class Side {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
  };

  SDL_Point rect_to_rect(const SDL_Rect& rect1, const SDL_Rect& rect2);
  bool collides(const SDL_Rect& rect, const std::vector<SDL_Rect>& rects, Side side, SDL_Point& point_contact);
  // bool rect_to_rects(const SDL_Rect& rect, const std::vector<SDL_Rect>& rects);
};

#endif
