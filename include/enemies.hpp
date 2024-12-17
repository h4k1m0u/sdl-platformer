#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include "texture.hpp"
#include "types.hpp"

class Enemies {
public:
  Enemies() = default;
  Enemies(SDL_Renderer* renderer, const std::vector<PatrolTrajectory>& patrol_trajectories);
  void render(int frame, const SDL_Rect& camera);
  void free();

private:
  SDL_Renderer* m_renderer;
  Texture m_texture;
  std::vector<PatrolTrajectory> m_patrol_trajectories;
  size_t m_n_enemies;
  std::vector<Direction> m_directions;
  std::vector<int> m_velocities_x;
  std::unordered_map<int, SDL_Rect> m_bboxes;

  static const int N_FRAMES = 3;
  Clips<N_FRAMES> m_positions_clips;

  void calculate_positions_clips();
  void calculate_bboxes(const std::vector<PatrolTrajectory>& patrol_trajectories);
};

#endif // ENEMIES_HPP
