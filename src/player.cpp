// #include <iostream>
#include <algorithm>
#include <array>

#include "player.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "collision.hpp"
#include "drawer.hpp"

const std::string PATH_TEXTURE = "./assets/werewolf-NESW.png";
const int WIDTH = 48;
const int HEIGHT = 46;
const int SPEED = 4;

/**
 * Gravity: friction to upward vertical movement in case of jump (oblique or not)
 * Impulse: factor to initial vertical push (case of jump) to resist gravity
 */
const int GRAVITY = 1;
const int IMPULSE_FACTOR_Y = 4;

Player::Player(SDL_Renderer* renderer, const std::vector<SDL_Rect>& bboxes_ground):
  // TODO: no need for this if collision detection made external???
  m_bboxes_ground(bboxes_ground),

  m_renderer(renderer),
  m_texture(PATH_TEXTURE, { WIDTH, HEIGHT }, renderer),
  m_position({ 200, 50 }),
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

/* confine within screen width */
int Player::clamp_x(int x_new) {
  return std::clamp(x_new, 0, Constants::LEVEL_WIDTH - WIDTH);
}

void Player::move_left() {
  m_direction = Direction::LEFT;
  m_velocity_x = -SPEED;
}

void Player::move_right() {
  m_direction = Direction::RIGHT;
  m_velocity_x = SPEED;
}

/**
 * Allow horizontal movement only if on ground
 * @param key_states Smoother results with keystates: move as long as key pressed (like joystick). SDL_KEYDOWN/UP better for puncutual events like firing a bullet (typing keyboard)
 * @param clicked Player can react to a click/touch on an arrow button
 */
void Player::handle_event(const Uint8* key_states, const std::unordered_map<Button, bool>& clicked) {
  // std::cout << "handle_event(): " << "m_velocity_y: " << m_velocity_y << '\n';
  std::string keys = "";

  m_direction = Direction::NONE;
  m_velocity_x = 0;
  m_velocity_y = 0;

  // vertical movement or oblique (if left/right also pressed)
  if (key_states[SDL_SCANCODE_UP] || key_states[SDL_SCANCODE_W] || clicked.at(Button::UP)) {
    jump();
    keys += "U";
  }

  if (key_states[SDL_SCANCODE_LEFT] || key_states[SDL_SCANCODE_A] || clicked.at(Button::LEFT)) {
    move_left();
    keys += "L";
  }
  else if (key_states[SDL_SCANCODE_RIGHT] || key_states[SDL_SCANCODE_D] || clicked.at(Button::RIGHT)) {
    move_right();
    keys += "R";
  }

  /*
  std::cout << "--- Keys: " << keys
            << " m_velocity_x: " << m_velocity_x
            << " m_velocity_y: " << m_velocity_y
            << '\n';
  */

  // TODO: should check for collision on left & right! (to re-position player on x-axis)

  // update position & bbox
  m_bbox.x = m_position.x = clamp_x(m_position.x + m_velocity_x);
}

/**
 * Check collision of next position
 * Used to prevent sprite from penetrating ground from top (on fall) or bottom (on jump)
 */
Collision::Sides Player::check_collision_ground(SDL_Point& point_contact) {
  SDL_Rect bbox_new = m_bbox;
  bbox_new.y += m_velocity_y;
  Collision::Sides sides = Collision::find_collision_sides(bbox_new, m_bboxes_ground, point_contact);
  /*
  std::cout << "check_collision_ground(): sides: "
            << static_cast<char>(sides.first) << static_cast<char>(sides.second)
            << '\n';
  */

  return sides;
}

/**
 * @return key Key corresp. to coin/enemy to destroy
 */
bool Player::check_collision_entities(const std::unordered_map<int, SDL_Rect>& bboxes_entities, int& key) {
  bool collides = false;
  key = -1;

  for (const auto& [ k, bbox_entity ] : bboxes_entities) {
    collides = Collision::rect_to_rect(m_bbox, bbox_entity);

    if (collides) {
      key = k;
      break;
    }
  }

  return collides;
}

/* Ease-In (acceleration) */
void Player::fall() {
  // more realistic fall when vertical velocity affected by gravity
  m_direction = Direction::NONE;
  m_velocity_y += GRAVITY;

  // collision detection performed once for grids above/below
  SDL_Point point_contact;
  auto [ side_x, side_y ] = check_collision_ground(point_contact);
  /*
  std::cout << "fall():"
            << " m_velocity_x: " << m_velocity_x
            << " m_velocity_y: " << m_velocity_y << '\n';
  */

  int y_new = m_position.y + m_velocity_y;

  if (side_y == Collision::SideY::ABOVE) {
    // colliding during downward movement
    y_new = point_contact.y - HEIGHT;
  }
  else if (side_y == Collision::SideY::BELOW) {
    // colliding during upward movement (deceleration in jump impulse)
    y_new = point_contact.y;
    m_velocity_y = 0;
    // std::cout << "fall(): --- HITTING FROM BOTTOM ---" << '\n';
  }

  m_bbox.y = m_position.y = y_new;

  // needed for oblique jump (left-up or right-up)
  m_bbox.x = m_position.x = clamp_x(m_position.x + m_velocity_x);
}

/* Ease-Out (deceleration thanks to gravity in fall()) */
void Player::jump() {
  // strong vertical velocity to resist gravity (to a certain extent)
  m_direction = Direction::NONE;
  m_velocity_y = -IMPULSE_FACTOR_Y * SPEED;

  SDL_Point point_contact;
  auto [ side_x, side_y ] = check_collision_ground(point_contact);
  /*
  std::cout << "--- JUMPING ---:"
            << " m_velocity_x: " << m_velocity_x
            << " m_velocity_y: " << m_velocity_y
            << '\n';
  */

  int y_new = m_position.y + m_velocity_y;
  if (side_y == Collision::SideY::BELOW) {
    y_new = point_contact.y;
    m_velocity_y = 0;
    // std::cout << "jump(): --- HITTING FROM BOTTOM ---" << '\n';
  }
  m_bbox.y = m_position.y = y_new;
}

/**
 * @param frame Determines where to clip texture
 */
void Player::render(int frame, const SDL_Rect& camera) {
  if (m_direction != Direction::NONE) {
    // slow down animation (change image every 9 frames)
    int frame_player = (frame / (N_FRAMES * 3)) % N_FRAMES;
    m_position_clip = m_positions_clips[m_direction][frame_player];
  }

  // change of origin rel. to camera
  SDL_Point position_rel = { m_position.x - camera.x, m_position.y - camera.y };
  m_texture.render(position_rel, m_position_clip);

  // show bbox in debug mode
  if (Constants::DEBUG) {
    Drawer::draw_bbox(m_renderer, m_bbox, &camera);
  }
}

SDL_Point Player::get_center() const {
  SDL_Point center = { m_position.x + WIDTH / 2, m_position.y + HEIGHT / 2 };
  return center;
}

void Player::free() {
  m_texture.free();
}
