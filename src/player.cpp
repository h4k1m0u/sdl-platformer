#include "player.hpp"

Player::Player(const Texture& texture):
  m_texture(texture),
  m_position({ 0, 0 }),
  m_velocity({ 0, 0 })
{
}

void Player::handle_event(const SDL_Event* e) {
  if (e->type == SDL_KEYUP) {
      m_velocity = { 0, 0 };
  }
  else if (e->type == SDL_KEYDOWN) {
    if (e->key.keysym.sym == SDLK_UP) {
      m_velocity = { 0, -SPEED };
    }
    else if (e->key.keysym.sym == SDLK_DOWN) {
      m_velocity = { 0, SPEED };
    }
    else if (e->key.keysym.sym == SDLK_LEFT) {
      m_velocity = { -SPEED, 0 };
    }
    else if (e->key.keysym.sym == SDLK_RIGHT) {
      m_velocity = { SPEED, 0 };
    }
  }

  m_position = { m_position.x + m_velocity.x, m_position.y + m_velocity.y };
}

void Player::render() {
  m_texture.render(m_position);
}
