#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <unordered_map>
#include <vector>

#include "collision.hpp"
#include "texture.hpp"
#include "button.hpp"
#include "types.hpp"
#include "timer_cooldown.hpp"

class Player {
private:
  static const int N_FRAMES = 3;

  std::vector<SDL_Rect> m_bboxes_ground;
  SDL_Renderer* m_renderer;
  Texture m_texture;
  SDL_Point m_position;
  SDL_Point m_position_clip;
  int m_velocity_x;
  int m_velocity_y;
  int m_n_lives;
  Direction m_direction;
  Clips<N_FRAMES> m_positions_clips;
  SDL_Rect m_bbox;

  void calculate_positions_clips();
  int clamp_x(int x_new);
  void move_left();
  void move_right();

public:
  Player() = default;
  Player(SDL_Renderer* renderer, const std::vector<SDL_Rect>& bboxes_ground);
  void handle_events(const Uint8* key_states, const std::unordered_map<Button, bool>& clicked);
  void render(int frame, const SDL_Rect& camera);
  void free();

  Collision::Sides check_collision_ground(SDL_Point& point_contact);
  bool check_collision_entities(const BboxesMap& bboxes_entities, int& key);
  bool check_collision_enemies(const BboxesMap& bboxes_enemies, const std::unordered_map<int, TimerCooldown>& timers_enemies, int& key, bool& kill_enemy);

  void jump();
  void fall();
  SDL_Point get_center() const;
  int get_n_lives() const;
  bool is_alive() const;
};

#endif
