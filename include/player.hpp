#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <unordered_map>
#include <array>
#include <vector>

#include "texture.hpp"
#include "direction.hpp"

class Player {
private:
  const std::string PATH_TEXTURE = "./assets/werewolf-NESW.png";
  const int WIDTH = 48;
  const int HEIGHT = 46;
  const int SPEED = 4;
  const int GRAVITY = 1;
  static const int N_FRAMES = 3;

  std::vector<SDL_Rect> m_obstacles;

  Texture m_texture;
  SDL_Point m_position;
  SDL_Point m_position_clip;
  int m_velocity_x;
  int m_velocity_y;
  Direction m_direction;

  using Frames = std::array<SDL_Point, N_FRAMES>;
  using Clips = std::unordered_map<Direction, Frames>;

  Clips m_positions_clips;

  SDL_Rect m_bbox;

  void calculate_positions_clips();
  bool collides_bottom(SDL_Point& point_contact);

public:
  Player(SDL_Renderer* renderer, const std::vector<SDL_Rect>& obstacles);
  void handle_event(const Uint8* key_states);
  void render(int frame);
  void free();

  void jump();
  void fall();
};

#endif
