#ifndef COIN_HPP
#define COIN_HPP

#include <array>

#include "texture.hpp"

class Coin {
public:
  Coin() = default;
  Coin(SDL_Renderer* renderer);

  void render(int frame);
  void free();

private:
  Texture m_texture;

  static const int N_FRAMES = 6;
  std::array<SDL_Point, N_FRAMES> m_positions_clips;

  void calculate_positions_clips();
};

#endif // COIN_HPP
