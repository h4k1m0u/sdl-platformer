#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include "texture.hpp"
#include "types.hpp"

class Enemies {
public:
  Enemies() = default;
  Enemies(SDL_Renderer* renderer, const std::vector<PatrolTrajectory>& patrol_trajectories);
  void render(int frame);
  void free();

private:
  Texture m_texture;
  std::vector<PatrolTrajectory> m_patrol_trajectories;
  std::unordered_map<int, SDL_Rect> m_bboxes;
  int m_velocity_x;

  static const int N_FRAMES = 3;
  Clips<N_FRAMES> m_positions_clips;

  void calculate_positions_clips();
  void calculate_bboxes(const std::vector<PatrolTrajectory>& patrol_trajectories);
};

#endif // ENEMIES_HPP
