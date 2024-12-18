#ifndef TEXT_HPP
#define TEXT_HPP

#include "texture.hpp"

class Text {
private:
  TTF_Font* m_font;
  Texture m_texture_score;
  Texture m_texture_lives;

public:
  Text() = default;
  Text(TTF_Font* font, SDL_Renderer* renderer);
  void render();
  void free();
  void set_score(int score);
  void set_lives(int n_lives);
};

#endif
