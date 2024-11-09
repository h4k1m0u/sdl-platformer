#include "obstacle.hpp"

Obstacle::Obstacle(SDL_Renderer* renderer, const SDL_Point& position):
  m_renderer(renderer),
  m_bbox({ position.x, position.y, WIDTH, HEIGHT })
{
}

SDL_Rect Obstacle::get_bbox() const {
  return m_bbox;
}

void Obstacle::render() {
  SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderFillRect(m_renderer, &m_bbox);
}
