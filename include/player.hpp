#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "texture.hpp"

class Player {
public:
  Player(const Texture& texture);
  void handle_event(const SDL_Event* e);
  void render();

private:
  Texture m_texture;
  SDL_Point m_position;
  SDL_Point m_velocity;

  const int SPEED = 10;
};

#endif
