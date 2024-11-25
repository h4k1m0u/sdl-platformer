#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <SDL2/SDL_render.h>

namespace Drawer {
  void draw_bbox(SDL_Renderer* renderer, const SDL_Rect& bbox, const SDL_Rect& camera);
}
#endif
