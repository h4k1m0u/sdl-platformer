#include "collision.hpp"

/* Collision detection between two AABBOX */
bool Collision::rect_to_rect(const SDL_Rect& rect1, const SDL_Rect& rect2) {
  SDL_Point min1 = { rect1.x, rect1.y };
  SDL_Point max1 = { rect1.x + rect1.w, rect1.y + rect1.h };

  SDL_Point min2 = { rect2.x, rect2.y };
  SDL_Point max2 = { rect2.x + rect2.w, rect2.y + rect2.h };

  bool collides_x = min2.x < max1.x && max2.x > min1.x;
  bool collides_y = min2.y < max1.y && max2.y > min1.y;

  return collides_x && collides_y;
}
