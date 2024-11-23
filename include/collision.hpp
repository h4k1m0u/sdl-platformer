#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <SDL2/SDL_rect.h>
#include <vector>

namespace Collision {
  enum class SideX : char {
    NONE = 'n',
    LEFT = 'l',
    RIGHT = 'r',
  };
  enum class SideY : char {
    NONE = 'n',
    ABOVE = 'a',
    BELOW = 'b',
  };

  using Sides = std::pair<SideX, SideY>;

  SDL_Point rect_to_rect(const SDL_Rect& rect1, const SDL_Rect& rect2);
  Sides find_collision_sides(const SDL_Rect& rect, const std::vector<SDL_Rect>& rects, SDL_Point& point_contact);
};

#endif
