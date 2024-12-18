#include "text.hpp"
#include "constants.hpp"

const SDL_Point POSITION_SCORE = { 0, 0 };

Text::Text(TTF_Font* font, SDL_Renderer* renderer):
  m_font(font),
  m_texture_score(font, "FPS: 0", renderer),
  m_texture_lives(font, "Lives: " + std::to_string(Constants::N_LIVES), renderer)
{
}

void Text::set_score(int score) {
  const std::string text = "Score: " + std::to_string(score);
  m_texture_score.set_from_text(m_font, text);
}

void Text::set_lives(int n_lives) {
  const std::string text = "Lives: " + std::to_string(n_lives);
  m_texture_lives.set_from_text(m_font, text);
}

void Text::render() {
  SDL_Point position_lives = { Constants::SCREEN_WIDTH - m_texture_lives.get_width(), 0 };
  m_texture_lives.render(position_lives);
  m_texture_score.render(POSITION_SCORE);
}

void Text::free() {
  m_texture_score.free();
  m_texture_lives.free();
}
