#include <iostream>
#include <algorithm>

#include "player.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "collision.hpp"

Player::Player(SDL_Renderer* renderer, const std::vector<SDL_Rect>& obstacles):
  // TODO: no need for this if collision detection made external???
  m_obstacles(obstacles),

  m_texture(PATH_TEXTURE, { WIDTH, HEIGHT }, renderer),
  m_position({ 100, 50 }),
  m_position_clip({ 0, 0 }),
  m_velocity_x(0),
  m_velocity_y(0),
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
 * Allow horizontal movement only if on ground
 * Smoother results with keystates: move as long as key pressed (like joystick)
 * SDL_KEYDOWN/UP better for puncutual events like firing a bullet (typing keyboard)
 */
void Player::handle_event(const Uint8* key_states) {
  m_velocity_y = 0;
  std::cout << "handle_event(): " << "m_velocity_y: " << m_velocity_y << '\n';

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

/**
 * Check collision of next position
 * Used to prevent sprite from penetrating ground from top (on fall) or bottom (on jump)
 */
bool Player::check_collision(Collision::Side side, SDL_Point& point_contact) {
  SDL_Rect bbox_new = m_bbox;
  bbox_new.y += m_velocity_y;
  bool collides = Collision::collides(bbox_new, m_obstacles, side, point_contact);
  std::cout << "check_collision(): " << collides << '\n';

  return collides;
}

/**
 * More realistic fall when velocity affected by gravity
 * Ease-In (acceleration)
 */
void Player::fall() {
  m_direction = Direction::NONE;
  m_velocity_y += GRAVITY;

  // TODO: collision detection should be performed only once!
  SDL_Point point_contact_top, point_contact_bottom;
  bool collides_top = check_collision(Collision::Side::TOP, point_contact_top);
  bool collides_bottom = check_collision(Collision::Side::BOTTOM, point_contact_bottom);
  std::cout << "fall(): " << "on_ground: " << collides_top
            << " GRAVITY: " << GRAVITY
            << " m_velocity_y: " << m_velocity_y << '\n';

  int y_new = m_bbox.y + m_velocity_y;

  if (collides_top)
    // colliding during downward movement
    y_new = point_contact_top.y - HEIGHT;
  else if (collides_bottom) {
    // colliding during upward movement (deceleration in jump impulse)
    y_new = point_contact_bottom.y;
    m_velocity_y = 0;
    std::cout << "fall(): --- HITTING FROM BOTTOM ---" << '\n';
  }

  m_bbox.y = m_position.y = y_new;
}

/**
 * Ease-Out (deceleration thanks to gravity in fall())
 */
void Player::jump() {
  // strong vertical velocity to resist gravity (to a certain extent)
  m_direction = Direction::NONE;
  m_velocity_y = -4 * SPEED;

  SDL_Point point_contact;
  bool collides_bottom = check_collision(Collision::Side::BOTTOM, point_contact);
  std::cout << "--- JUMPING ---: m_velocity_y: " << m_velocity_y
            << " collides: " << collides_bottom
            << '\n';

  int y_new = m_position.y + m_velocity_y;
  if (collides_bottom) {
    y_new = point_contact.y;
    m_velocity_y = 0;
    std::cout << "jump(): --- HITTING FROM BOTTOM ---" << '\n';
  }
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
