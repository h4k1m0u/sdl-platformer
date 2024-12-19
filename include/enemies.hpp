#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include "texture.hpp"
#include "types.hpp"
#include "timer_cooldown.hpp"

class Enemies {
public:
  Enemies() = default;
  Enemies(SDL_Renderer* renderer, const std::vector<PatrolTrajectory>& patrol_trajectories);
  const BboxesMap& get_bboxes() const;
  const std::unordered_map<int, TimerCooldown>& get_timers() const;
  void render(int frame, const SDL_Rect& camera);
  void destroy(int key);
  void free();

  void start_cooldown_timer(int key);
  void stop_cooldown_timer(int key);
  bool is_cool(int key);

private:
  SDL_Renderer* m_renderer;
  Texture m_texture;
  std::vector<PatrolTrajectory> m_patrol_trajectories;
  size_t m_n_enemies;
  std::vector<Direction> m_directions;
  std::vector<int> m_velocities_x;
  BboxesMap m_bboxes;
  std::unordered_map<int, TimerCooldown> m_timers_cooldown;

  static const int N_FRAMES = 3;
  Clips<N_FRAMES> m_positions_clips;

  void calculate_positions_clips();
  void calculate_bboxes();
  void set_timers_cooldown();
};

#endif // ENEMIES_HPP
