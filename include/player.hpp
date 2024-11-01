#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <unordered_map>
#include <array>

#include "texture.hpp"
#include "direction.hpp"

class Player {
private:
  Texture m_texture;
  SDL_Point m_position;
  SDL_Point m_position_clip;
  SDL_Point m_velocity;
  Direction m_direction;

  static const int WIDTH = 48;
  static const int HEIGHT = 64;
  static const int SPEED = 10;
  static const int N_FRAMES = 3;

  using Frames = std::array<SDL_Point, N_FRAMES>;
  using Clips = std::unordered_map<Direction, Frames>;

  Clips m_positions_clips;

public:
  Player(SDL_Renderer* renderer);
  void calculate_positions_clips();
  void handle_event(const SDL_Event* e);
  void render(int frame);
  void free();
};

#endif
