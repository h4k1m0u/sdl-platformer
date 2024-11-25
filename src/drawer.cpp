#include "drawer.hpp"
#include "constants.hpp"

/* Used for debugging */
void Drawer::draw_bbox(SDL_Renderer* renderer, const SDL_Rect& bbox, const SDL_Rect& camera) {
  // change of origin rel. to camera
  SDL_Rect bbox_rel = bbox;
  bbox_rel.x = bbox.x - camera.x;
  bbox_rel.y = bbox.y - camera.y;

  const SDL_Color color = Constants::COLOR_BBOX;
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawRect(renderer, &bbox_rel);
}
