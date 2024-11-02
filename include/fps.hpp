#ifndef FPS_HPP
#define FPS_HPP

#include "texture.hpp"

class FPS {
private:
  SDL_Point m_position;
  TTF_Font* m_font;
  Texture m_texture;
  Uint64 m_start_ticks;

public:
  FPS(const SDL_Point& position, TTF_Font* font, SDL_Renderer* renderer);
  void start_timer();
  void calculate(int frame);
  void render();
  void free();
};

#endif
