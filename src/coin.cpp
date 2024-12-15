#include "coin.hpp"

const std::string PATH_TEXTURE = "./assets/coin.png";

const int WIDTH = 18;
const int HEIGHT = 20;

Coin::Coin(SDL_Renderer* renderer, const std::vector<SDL_Point>& positions):
  m_texture(PATH_TEXTURE, { WIDTH, HEIGHT }, renderer),
  m_positions(positions)
{
  calculate_positions_clips();
}

void Coin::calculate_positions_clips() {
  for (int i = 0; i < N_FRAMES; ++i) {
    m_positions_clips[i] = { i * WIDTH, 0 };
  }
}

void Coin::render(int frame, const SDL_Rect& camera) {
  // slow down animation (change image every 6 frames)
  int frame_coin = (frame / N_FRAMES) % N_FRAMES;
  SDL_Point position_clip = m_positions_clips[frame_coin];

  // change of origin rel. to camera
  for (const SDL_Point& position : m_positions) {
    SDL_Point position_rel = { position.x - camera.x, position.y - camera.y };
    m_texture.render(position_rel, position_clip);
  }
}

void Coin::free() {
  m_texture.free();
}
