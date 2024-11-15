#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <unordered_map>
#include <array>

#include "texture.hpp"
#include "direction.hpp"
#include "obstacle.hpp"

class Player {
private:
  const std::string PATH_TEXTURE = "./assets/werewolf-NESW.png";
  const int WIDTH = 48;
  const int HEIGHT = 64;
  const int SPEED = 5;
  static const int N_FRAMES = 3;

  Texture m_texture;
  SDL_Point m_position;
  SDL_Point m_position_clip;
  SDL_Point m_velocity;
  Direction m_direction;

  using Frames = std::array<SDL_Point, N_FRAMES>;
  using Clips = std::unordered_map<Direction, Frames>;

  Clips m_positions_clips;

  SDL_Rect m_bbox;

  void calculate_positions_clips();

public:
  Player(SDL_Renderer* renderer);
  void handle_event(const Uint8* key_states, const Obstacle& obstacle);
  void render(int frame);
  void free();
};

#endif
