#include "sdl_utils.hpp"

bool SDLUtils::operator==(const SDL_Point& point1, const SDL_Point& point2) {
  return point1.x == point2.x && point1.y == point2.y;
}
