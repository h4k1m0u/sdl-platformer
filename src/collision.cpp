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
 * @pararm side Position of rect rel. to rect_other (e.g. TOP: rect is above rect_other)
 * @return contact_point Needed to re-position sprite on collision (and avoid penetrating tiles)
 */
bool Collision::collides(const SDL_Rect& rect, const std::vector<SDL_Rect>& rects, Side side, SDL_Point& point_contact) {
  for (const SDL_Rect& rect_other : rects) {
    SDL_Point collides = rect_to_rect(rect, rect_other);

    bool is_above = rect.y < rect_other.y;
    bool is_left = rect.x < rect_other.x;

    bool is_correct_side = (
      (side == Side::TOP && is_above) ||
      (side == Side::BOTTOM && !is_above) ||
      (side == Side::LEFT && is_left) ||
      (side == Side::RIGHT && !is_left)
    );

    if (collides.x && collides.y && is_correct_side) {
      if (side == Side::TOP)
        point_contact.y = rect_other.y;
      else if (side == Side::BOTTOM)
        point_contact.y = rect_other.y + rect_other.h;
      else if (side == Side::LEFT)
        point_contact.x = rect_other.x;
      else if (side == Side::RIGHT)
        point_contact.x = rect_other.x + rect_other.w;

      return true;
    }
  }

  point_contact = { -1, -1 };
  return false;
}
