#include "enemies.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "drawer.hpp"
#include "types.hpp"

const std::string PATH_TEXTURE = "./assets/zombie.png";

const int WIDTH_ENEMY = 32;
const int HEIGHT_ENEMY = 48;
const int SPEED = 2;

Enemies::Enemies(SDL_Renderer* renderer, const std::vector<PatrolTrajectory>& patrol_trajectories):
  m_renderer(renderer),
  m_texture(PATH_TEXTURE, { WIDTH_ENEMY, HEIGHT_ENEMY }, renderer),
  m_patrol_trajectories(patrol_trajectories),
  m_n_enemies(patrol_trajectories.size()),
  m_directions(m_n_enemies, Direction::RIGHT),
  m_velocities_x(m_n_enemies, SPEED)
{
  calculate_bboxes();
  calculate_positions_clips();
  set_timers_cooldown();
}

void Enemies::calculate_bboxes() {
  // shift sprite up as its height > height tile
  int delta_y = HEIGHT_ENEMY - Constants::HEIGHT_TILE;

  for (size_t key = 0; key < m_n_enemies; ++key) {
    auto [ position_start, position_end ] = m_patrol_trajectories[key];
    SDL_Point position = { position_start.x, position_start.y - delta_y };
    m_bboxes[key] = { position.x, position.y, WIDTH_ENEMY, HEIGHT_ENEMY };
  }
}

/* Used to avoid detecting collision with player each frame (& kill latter immediately) */
void Enemies::set_timers_cooldown() {
  for (size_t key = 0; key < m_n_enemies; ++key) {
    m_timers_cooldown[key];
  }
}

void Enemies::start_cooldown_timer(int key) {
  m_timers_cooldown[key].start();
}

void Enemies::stop_cooldown_timer(int key) {
  m_timers_cooldown[key].stop();
}

bool Enemies::is_cool(int key) {
  return m_timers_cooldown[key].is_cool();
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

const BboxesMap& Enemies::get_bboxes() const {
  return m_bboxes;
}

/* Returned as a ref. (not const ref.) since timer is modified in player class */
const std::unordered_map<int, TimerCooldown>& Enemies::get_timers() const {
  return m_timers_cooldown;
}

/* hashmaps have constant-time complexity for removal (linear for vectors) */
void Enemies::destroy(int key) {
  m_bboxes.erase(key);
}

void Enemies::render(int frame, const SDL_Rect& camera) {
  // slow down animation (change image every 9 frames)
  int frame_coin = (frame / (N_FRAMES * 3)) % N_FRAMES;

  for (auto& [ key, bbox ] : m_bboxes) {
    const auto& [ start, end ] = m_patrol_trajectories[key];
    Direction& direction = m_directions[key];
    int& velocity_x = m_velocities_x[key];

    // change direction when reaching patrol points
    SDL_Point position = { bbox.x, bbox.y };

    if (position.x >= end.x && direction == Direction::RIGHT) {
      direction = Direction::LEFT;
      velocity_x = -SPEED;
    }
    else if (position.x <= start.x && direction == Direction::LEFT) {
      direction = Direction::RIGHT;
      velocity_x = SPEED;
    }

    SDL_Point position_clip = m_positions_clips[direction][frame_coin];
    SDL_Point position_rel = { position.x - camera.x, position.y - camera.y };
    m_texture.render(position_rel, position_clip);

    // show bbox in debug mode
    if (Constants::DEBUG) {
      Drawer::draw_bbox(m_renderer, bbox, &camera);
    }

    bbox.x = bbox.x + velocity_x;
  }
}

void Enemies::free() {
  m_texture.free();
}
