#include <iostream>

#include "coin.hpp"

const std::string PATH_TEXTURE = "./assets/coin.png";

const int WIDTH = 18;
const int HEIGHT = 20;

Coin::Coin(SDL_Renderer* renderer):
  m_texture(PATH_TEXTURE, { WIDTH, HEIGHT }, renderer)
{
  calculate_positions_clips();
}

void Coin::calculate_positions_clips() {
  for (int i = 0; i < N_FRAMES; ++i) {
    m_positions_clips[i] = { i * WIDTH, 0 };
    std::cout << "inserting: " << i << ": " << m_positions_clips[i].x << ", " << m_positions_clips[i].y << '\n';
  }
}

void Coin::render(int frame) {
  // slow down animation (change image every 6 frames)
  SDL_Point position = { 0, 0 };
  int frame_coin = (frame / N_FRAMES) % N_FRAMES;
  SDL_Point position_clip = m_positions_clips[frame_coin];

  m_texture.render(position, position_clip);
}

void Coin::free() {
  m_texture.free();
}
