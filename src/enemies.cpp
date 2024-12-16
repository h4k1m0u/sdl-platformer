#include <array>

#include "enemies.hpp"
#include "direction.hpp"

const std::string PATH_TEXTURE = "./assets/zombie.png";

const int WIDTH_ENEMY = 32;
const int HEIGHT_ENEMY = 48;
const int SPEED = 2;

Enemies::Enemies(SDL_Renderer* renderer, const std::vector<PatrolTrajectory>& patrol_trajectories):
  m_texture(PATH_TEXTURE, { WIDTH_ENEMY, HEIGHT_ENEMY }, renderer),
  m_patrol_trajectories(patrol_trajectories),
  m_velocity_x(SPEED)
{
  calculate_bboxes(patrol_trajectories);
  calculate_positions_clips();
}

void Enemies::calculate_bboxes(const std::vector<PatrolTrajectory>& patrol_trajectories) {
  for (size_t key = 0; key < patrol_trajectories.size(); ++key) {
    auto [ position_start, position_end ] = patrol_trajectories[key];
    m_bboxes[key] = { position_start.x, position_start.y, WIDTH_ENEMY, HEIGHT_ENEMY };
  }
}

void Enemies::calculate_positions_clips() {
  const std::array<int, N_FRAMES> XS = { 0, WIDTH_ENEMY, 2*WIDTH_ENEMY };
  const std::unordered_map<Direction, int> YS = {
    { Direction::LEFT,  0 },
    { Direction::RIGHT, HEIGHT_ENEMY },
  };

  for (const auto& [ direction, y ] : YS) {
    for (int i_frame = 0; i_frame < N_FRAMES; ++i_frame) {
      int x = XS[i_frame];
      m_positions_clips[direction][i_frame] = { x, y };
    }
  }
}

void Enemies::render(int frame) {
  // slow down animation (change image every 9 frames)
  int frame_coin = (frame / (N_FRAMES * 3)) % N_FRAMES;

  for (size_t key = 0; key < m_bboxes.size(); ++key) {
    // const auto& [ start, end ] = m_patrol_trajectories[key];
    SDL_Rect& bbox = m_bboxes[key];

    // Direction direction = end.x > start.x ? Direction::RIGHT : Direction::LEFT;
    Direction direction = Direction::RIGHT;
    SDL_Point position_clip = m_positions_clips[direction][frame_coin];
    SDL_Point position = { bbox.x, bbox.y };
    m_texture.render(position, position_clip);

    bbox.x = bbox.x + m_velocity_x;
  }
}

void Enemies::free() {
  m_texture.free();
}
