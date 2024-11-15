#include <algorithm>

#include "player.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "collision.hpp"

Player::Player(SDL_Renderer* renderer):
  m_texture(PATH_TEXTURE, { WIDTH, HEIGHT }, renderer),
  m_position({ 100, 100 }),
  m_position_clip({ 0, 0 }),
  m_velocity({ 0, 0 }),
  m_direction(Direction::NONE),
  m_bbox({ m_position.x, m_position.y, WIDTH, HEIGHT })
{
  calculate_positions_clips();
}

void Player::calculate_positions_clips() {
  const std::array<int, N_FRAMES> XS = { 0, WIDTH, 2*WIDTH };
  const std::unordered_map<Direction, int> YS = {
    { Direction::UP,          0 },
    { Direction::RIGHT,  HEIGHT },
    { Direction::DOWN, 2*HEIGHT },
    { Direction::LEFT, 3*HEIGHT },
  };

  for (const auto& [ direction, y ] : YS) {
    for (int i_frame = 0; i_frame < N_FRAMES; ++i_frame) {
      int x = XS[i_frame];
      m_positions_clips[direction][i_frame] = { x, y };
    }
  }
}

/**
 * Smoother results with keystates: move as long as key pressed (like joystick)
 * SDL_KEYDOWN/UP better for puncutual events like firing a bullet (typing keyboard)
 */
void Player::handle_event(const Uint8* key_states, const std::vector<SDL_Rect>& bboxes_obstacles) {
  if (key_states[SDL_SCANCODE_UP]) {
    m_direction = Direction::UP;
    m_velocity = { 0, -SPEED };
  }
  else if (key_states[SDL_SCANCODE_DOWN]) {
    m_direction = Direction::DOWN;
    m_velocity = { 0, SPEED };
  }
  else if (key_states[SDL_SCANCODE_LEFT]) {
    m_direction = Direction::LEFT;
    m_velocity = { -SPEED, 0 };
  }
  else if (key_states[SDL_SCANCODE_RIGHT]) {
    m_direction = Direction::RIGHT;
    m_velocity = { SPEED, 0 };
  } else {
    m_direction = Direction::NONE;
    m_velocity = { 0, 0 };
  }

  // confine within screen & update bbox
  int x_new = std::clamp(m_position.x + m_velocity.x, 0, Constants::SCREEN_WIDTH - WIDTH);
  int y_new = std::clamp(m_position.y + m_velocity.y, 0, Constants::SCREEN_HEIGHT - HEIGHT);

  // move only if no collision detected
  SDL_Rect bbox_player_new = { x_new, y_new, WIDTH, HEIGHT };
  bool collides = Collision::rect_to_rects(bbox_player_new, bboxes_obstacles);

  if (!collides) {
    m_bbox.x = m_position.x = x_new;
    m_bbox.y = m_position.y = y_new;
  }
}

/**
 * @param frame Determines where to clip texture
 */
void Player::render(int frame) {
  if (m_direction != Direction::NONE) {
    // slow down animation (change image every 9 frames)
    // int frame_player = frame % N_FRAMES;
    int frame_player = (frame / (N_FRAMES * 3)) % N_FRAMES;
    m_position_clip = m_positions_clips[m_direction][frame_player];
  }

  m_texture.render(m_position, m_position_clip);
}

void Player::free() {
  m_texture.free();
}
