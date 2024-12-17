#include <vector>

#include "arrow_buttons.hpp"
#include "constants.hpp"
#include "collision.hpp"

using namespace Constants;

const std::string PATH_TEXTURE = "./assets/arrow-buttons.png";

const int WIDTH_BUTTON = WIDTH_TILE * 4;
const int HEIGHT_BUTTON = WIDTH_TILE * 4;

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
  m_clicked({
    { Button::LEFT, false },
    { Button::RIGHT, false },
    { Button::UP, false },
  }),
  m_fingers()
{
}

void ArrowButtons::reset_all() {
  for (Button button : BUTTONS) {
    m_clicked[button] = false;
  }
}

void ArrowButtons::handle_mouse_event(const SDL_Event& e) {
  if (e.button.button != SDL_BUTTON_LEFT)
    return;

  if (e.type == SDL_MOUSEBUTTONUP) {
    reset_all();
    return;
  }

  SDL_Point mouse_position = { e.button.x, e.button.y };

  for (Button button : BUTTONS) {
    if (Collision::point_in_rect(mouse_position, RECTS.at(button))) {
      m_clicked[button] = true;
      break;
    }
  }
}

void ArrowButtons::reset_by_finger(SDL_FingerID finger_id) {
  Button button_released = m_fingers[finger_id];
  m_clicked[button_released] = false;
  m_fingers.erase(finger_id);
}

void ArrowButtons::handle_touch_event(const SDL_Event& e) {
  if (e.type == SDL_FINGERUP) {
    // only reset the finger that was released
    reset_by_finger(e.tfinger.fingerId);
    return;
  }

  // find fingers positions & ids
  SDL_TouchID touch_id = e.tfinger.touchId;
  int n_fingers = SDL_GetNumTouchFingers(touch_id);
  SDL_Point fingers_positions[n_fingers];
  SDL_FingerID fingers_ids[n_fingers];

  for (int i = 0; i < n_fingers; ++i) {
    SDL_Finger* finger = SDL_GetTouchFinger(touch_id, i);
    SDL_Point finger_position = {
      static_cast<int>(finger->x * Constants::SCREEN_WIDTH),
      static_cast<int>(finger->y * Constants::SCREEN_HEIGHT)
    };
    fingers_positions[i] = finger_position;
    fingers_ids[i] = finger->id;
  }

  // test each button against all fingers pressed
  for (Button button : BUTTONS) {
    for (int i = 0; i < n_fingers; ++i) {
      SDL_Point finger_position = fingers_positions[i];
      SDL_FingerID finger_id = fingers_ids[i];

      if (Collision::point_in_rect(finger_position, RECTS.at(button))) {
        m_clicked[button] = true;
        m_fingers[finger_id] = button;
        break;
      }
    }
  }
}

/* Handles both mouse clicks & mobile touch events */
void ArrowButtons::handle_event(const SDL_Event& e) {
  bool is_mouse_event = e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP;
  bool is_touch_event = e.type == SDL_FINGERUP || e.type == SDL_FINGERDOWN;

  if (is_mouse_event) {
    handle_mouse_event(e);
  }
  else if (is_touch_event) {
    handle_touch_event(e);
  }
}

const std::unordered_map<Button, bool>& ArrowButtons::get_clicked() const {
  return m_clicked;
}

void ArrowButtons::render() {
  for (Button button : BUTTONS) {
    SDL_Point position_clip = m_clicked[button] ? POSITIONS_CLIPS_PRESSED.at(button) : POSITIONS_CLIPS.at(button);
    SDL_Point position = POSITIONS.at(button);
    m_texture.render(position, position_clip);
  }
}

void ArrowButtons::free() {
  m_texture.free();
}
