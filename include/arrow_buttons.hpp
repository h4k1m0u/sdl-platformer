#ifndef ARROW_BUTTONS_HPP
#define ARROW_BUTTONS_HPP

#include <unordered_map>

#include "texture.hpp"
#include "button.hpp"

class ArrowButtons {
public:
  ArrowButtons() = default;
  ArrowButtons(SDL_Renderer* renderer);

  void handle_event(const SDL_Event& e);
  void render();
  void free();

private:
  Texture m_texture;

  std::unordered_map<Button, bool> m_is_clicked;
};

#endif // ARROW_BUTTONS_HPP
