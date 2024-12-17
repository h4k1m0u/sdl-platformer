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
  const std::unordered_map<Button, bool>& get_clicked() const;
  void render();
  void free();

private:
  Texture m_texture;
  std::unordered_map<Button, bool> m_clicked;
  std::unordered_map<SDL_FingerID, Button> m_fingers;

  void handle_mouse_event(const SDL_Event& e);
  void handle_touch_event(const SDL_Event& e);
  void reset_all();
  void reset_by_finger(SDL_FingerID fingerId);
};

#endif // ARROW_BUTTONS_HPP
