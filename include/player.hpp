#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <unordered_map>
#include <array>
#include <vector>

#include "collision.hpp"
#include "texture.hpp"
#include "direction.hpp"
#include "button.hpp"

class Player {
private:
  static const int N_FRAMES = 3;
  using Frames = std::array<SDL_Point, N_FRAMES>;
  using Clips = std::unordered_map<Direction, Frames>;

  std::vector<SDL_Rect> m_obstacles;
  SDL_Renderer* m_renderer;
  Texture m_texture;
  SDL_Point m_position;
  SDL_Point m_position_clip;
  int m_velocity_x;
  int m_velocity_y;
  Direction m_direction;
  Clips m_positions_clips;
  SDL_Rect m_bbox;

  void calculate_positions_clips();
  int clamp_x(int x_new);
  void move_left();
  void move_right();

public:
  Player() = default;
  Player(SDL_Renderer* renderer, const std::vector<SDL_Rect>& obstacles);
  void handle_event(const Uint8* key_states, const std::unordered_map<Button, bool>& clicked);
  void render(int frame, const SDL_Rect& camera);
  void free();

  Collision::Sides check_collision(SDL_Point& point_contact);
  void jump();
  void fall();
  SDL_Point get_center() const;
};

#endif
