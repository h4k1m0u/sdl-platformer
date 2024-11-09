#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <SDL2/SDL_render.h>

class Obstacle {
public:
  Obstacle(SDL_Renderer* renderer, const SDL_Point& position);
  SDL_Rect get_bbox() const;
  void render();

private:
  SDL_Renderer* m_renderer;
  SDL_Rect m_bbox;

  static const int WIDTH = 256;
  static const int HEIGHT = 64;
};

#endif
