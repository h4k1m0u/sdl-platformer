#include "player.hpp"
#include "direction.hpp"

Player::Player(SDL_Renderer* renderer):
  m_texture("./werewolf-NESW.png", { WIDTH, HEIGHT }, renderer),
  m_position({ 0, 0 }),
  m_position_clip({ 0, 0 }),
  m_velocity({ 0, 0 }),
  m_direction(Direction::NONE)
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

void Player::handle_event(const SDL_Event* e) {
  if (e->type == SDL_KEYUP) {
      m_direction = Direction::NONE;
      m_velocity = { 0, 0 };
  }
  else if (e->type == SDL_KEYDOWN) {
    if (e->key.keysym.sym == SDLK_UP) {
      m_direction = Direction::UP;
      m_velocity = { 0, -SPEED };
    }
    else if (e->key.keysym.sym == SDLK_DOWN) {
      m_direction = Direction::DOWN;
      m_velocity = { 0, SPEED };
    }
    else if (e->key.keysym.sym == SDLK_LEFT) {
      m_direction = Direction::LEFT;
      m_velocity = { -SPEED, 0 };
    }
    else if (e->key.keysym.sym == SDLK_RIGHT) {
      m_direction = Direction::RIGHT;
      m_velocity = { SPEED, 0 };
    }
  }

  m_position = { m_position.x + m_velocity.x, m_position.y + m_velocity.y };
}

/**
 * @param frame Determines where to clip texture
 */
void Player::render(int frame) {
  if (m_direction != Direction::NONE) {
    // slow down animation (change image every 6 frames)
    // int frame_player = frame % N_FRAMES;
    int frame_player = (frame / (N_FRAMES * 2)) % N_FRAMES;
    m_position_clip = m_positions_clips[m_direction][frame_player];
  }

  m_texture.render(m_position, m_position_clip);
}

void Player::free() {
  m_texture.free();
}
