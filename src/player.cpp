#include <iostream>
#include <algorithm>

#include "player.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "collision.hpp"

Player::Player(SDL_Renderer* renderer, const std::vector<SDL_Rect>& obstacles):
  m_obstacles(obstacles),

  m_texture(PATH_TEXTURE, { WIDTH, HEIGHT }, renderer),
  m_position({ 100, 50 }),
  m_position_clip({ 0, 0 }),
  m_velocity_x(0),
  m_velocity_y(0),
  m_direction(Direction::NONE),
  m_bbox({ m_position.x, m_position.y, WIDTH, HEIGHT }),
  m_can_jump(false)
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
 * Allow horizontal movement only if on ground
 * Smoother results with keystates: move as long as key pressed (like joystick)
 * SDL_KEYDOWN/UP better for puncutual events like firing a bullet (typing keyboard)
 */
void Player::handle_event(const Uint8* key_states) {
  SDL_Point point_contact;
  bool on_ground = collides_bottom(point_contact);

  if (!on_ground) {
    fall();
    return;
  }

  m_can_jump = true;
  m_velocity_y = 0;

  if (key_states[SDL_SCANCODE_LEFT] || key_states[SDL_SCANCODE_A]) {
    m_direction = Direction::LEFT;
    m_velocity_x = -SPEED;
  }
  else if (key_states[SDL_SCANCODE_RIGHT] || key_states[SDL_SCANCODE_D]) {
    m_direction = Direction::RIGHT;
    m_velocity_x = SPEED;
  } else {
    m_direction = Direction::NONE;
    m_velocity_x = 0;
  }

  // TODO: should check for collision on left & right! (to re-position player on x-axis)

  // confine within screen & update bbox
  int x_new = std::clamp(m_position.x + m_velocity_x, 0, Constants::SCREEN_WIDTH - WIDTH);
  m_bbox.x = m_position.x = x_new;
}

/* Check collision of next position to prevent sprite from penetrating ground */
bool Player::collides_bottom(SDL_Point& point_contact) {
  SDL_Rect bbox_new = m_bbox;
  bbox_new.y += m_velocity_y;
  const Collision::Side side = Collision::Side::TOP;

  return Collision::collides(bbox_new, m_obstacles, side, point_contact);
}

/**
 * More realistic fall when velocity affected by gravity
 * Ease-In (acceleration)
 */
void Player::fall() {
  m_direction = Direction::NONE;
  m_velocity_y += GRAVITY;
  std::cout << "m_velocity_y: " << m_velocity_y << '\n';

  SDL_Point point_contact;
  bool on_ground = collides_bottom(point_contact);
  std::cout << "on_ground: " << on_ground << " point_contact.y: " << point_contact.y << '\n';

  int y_new = on_ground ? point_contact.y - HEIGHT : m_bbox.y + m_velocity_y;
  m_bbox.y = m_position.y = y_new;
}

/**
 * Ease-Out (deceleration thanks to gravity in fall())
 */
void Player::jump() {
  // prevent player from jumping while in the air
  if (!m_can_jump)
    return;

  m_can_jump = false;

  // strong vertical velocity to resist gravity (to a certain extent)
  std::cout << "--- JUMPING ---" << '\n';
  m_direction = Direction::NONE;
  m_velocity_y = -4 * SPEED;
  int y_new = m_position.y + m_velocity_y;
  m_bbox.y = m_position.y = y_new;
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
