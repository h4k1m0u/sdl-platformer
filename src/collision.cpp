#include "collision.hpp"

/* Collision detection between two AABBOX */
SDL_Point Collision::rect_to_rect(const SDL_Rect& rect1, const SDL_Rect& rect2) {
  SDL_Point min1 = { rect1.x, rect1.y };
  SDL_Point max1 = { rect1.x + rect1.w, rect1.y + rect1.h };

  SDL_Point min2 = { rect2.x, rect2.y };
  SDL_Point max2 = { rect2.x + rect2.w, rect2.y + rect2.h };

  bool collides_x = min2.x <= max1.x && max2.x >= min1.x;
  bool collides_y = min2.y <= max1.y && max2.y >= min1.y;
  SDL_Point collides = { collides_x, collides_y };

  return collides;
}

/**
 * Check if colliding on given side & return contact point (y for TB & x for LR)
 * @return side Position of rect rel. to rect_other if colliding (e.g. TOP: rect is above rect_other)
 * @return contact_point Needed to re-position sprite on collision (and avoid penetrating tiles)
 */
Collision::Sides Collision::find_collision_sides(const SDL_Rect& rect, const std::vector<SDL_Rect>& rects, SDL_Point& point_contact) {

  for (const SDL_Rect& rect_other : rects) {
    SDL_Point collides = rect_to_rect(rect, rect_other);

    if (!collides.x || !collides.y)
      continue;

    point_contact = { -1, -1 };
    Sides side(SideX::NONE, SideY::NONE);

    if (rect.x < rect_other.x) {
      point_contact.x = rect_other.x;
      side.first = SideX::LEFT;
    }
    else if (rect.x + rect.w > rect_other.x + rect_other.w) {
      point_contact.x = rect_other.x + rect_other.w;
      side.first = SideX::RIGHT;
    }

    if (rect.y < rect_other.y) {
      point_contact.y = rect_other.y;
      side.second = SideY::ABOVE;
    }
    else if (rect.y + rect.h > rect_other.y + rect_other.h) {
      point_contact.y = rect_other.y + rect_other.h;
      side.second = SideY::BELOW;
    }

    return side;
  }

  point_contact = { -1, -1 };
  return std::make_pair(SideX::NONE, SideY::NONE);
}
