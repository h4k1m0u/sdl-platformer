#include <vector>

#include "arrow_buttons.hpp"
#include "constants.hpp"
#include "collision.hpp"

using namespace Constants;

const std::string PATH_TEXTURE = "./assets/arrow-buttons.png";

const int WIDTH_BUTTON = WIDTH_TILE * 2;
const int HEIGHT_BUTTON = WIDTH_TILE * 2;

const std::unordered_map<Button, SDL_Point> POSITIONS_CLIPS = {
  { Button::LEFT,  { 0, 0 } },
  { Button::RIGHT, { 0, HEIGHT_BUTTON } },
  { Button::UP,    { 0, HEIGHT_BUTTON * 2 } },
};

const std::unordered_map<Button, SDL_Point> POSITIONS_CLIPS_PRESSED = {
  { Button::LEFT,  { WIDTH_BUTTON, 0 } },
  { Button::RIGHT, { WIDTH_BUTTON, HEIGHT_BUTTON } },
  { Button::UP,    { WIDTH_BUTTON, HEIGHT_BUTTON * 2 } },
};

const std::unordered_map<Button, SDL_Point> POSITIONS = {
  { Button::LEFT,  { WIDTH_TILE,                                 SCREEN_HEIGHT - (HEIGHT_TILE + HEIGHT_BUTTON) } },
  { Button::RIGHT, { WIDTH_TILE*2 + WIDTH_BUTTON,                SCREEN_HEIGHT - (HEIGHT_TILE + HEIGHT_BUTTON) } },
  { Button::UP,    { SCREEN_WIDTH - (WIDTH_TILE + WIDTH_BUTTON), SCREEN_HEIGHT - (HEIGHT_TILE + HEIGHT_BUTTON) } },
};

const std::unordered_map<Button, SDL_Rect> RECTS = {
  { Button::LEFT,  { POSITIONS.at(Button::LEFT).x, POSITIONS.at(Button::LEFT).y, WIDTH_BUTTON, HEIGHT_BUTTON } },
  { Button::RIGHT, { POSITIONS.at(Button::RIGHT).x, POSITIONS.at(Button::RIGHT).y, WIDTH_BUTTON, HEIGHT_BUTTON } },
  { Button::UP,    { POSITIONS.at(Button::UP).x, POSITIONS.at(Button::UP).y, WIDTH_BUTTON, HEIGHT_BUTTON } },
};

const std::vector<Button> BUTTONS = { Button::LEFT, Button::RIGHT, Button::UP };

ArrowButtons::ArrowButtons(SDL_Renderer* renderer):
  m_texture(PATH_TEXTURE, { WIDTH_BUTTON, HEIGHT_BUTTON }, renderer),
  m_is_clicked({
    { Button::LEFT, false },
    { Button::RIGHT, false },
    { Button::UP, false },
  })
{
}

void ArrowButtons::handle_event(const SDL_Event& e) {
  if (e.button.button != SDL_BUTTON_LEFT)
    return;

  if (e.type == SDL_MOUSEBUTTONUP) {
    for (Button button : BUTTONS) {
      m_is_clicked[button] = false;
    }
    return;
  }

  for (size_t i = 0; i < BUTTONS.size(); ++i) {
    Button button = static_cast<Button>(i);
    SDL_Point mouse = { e.button.x, e.button.y };

    if (Collision::point_in_rect(mouse, RECTS.at(button))) {
      m_is_clicked[button] = true;
      break;
    }
  }
}

void ArrowButtons::render() {
  for (Button button : BUTTONS) {
    SDL_Point position_clip = m_is_clicked[button] ? POSITIONS_CLIPS_PRESSED.at(button) : POSITIONS_CLIPS.at(button);
    SDL_Point position = POSITIONS.at(button);
    m_texture.render(position, position_clip);
  }
}

void ArrowButtons::free() {
  m_texture.free();
}
