#ifndef COIN_HPP
#define COIN_HPP

#include <array>
#include <vector>

#include "texture.hpp"

class Coin {
public:
  Coin() = default;
  Coin(SDL_Renderer* renderer, const std::vector<SDL_Point>& positions);
  void render(int frame, const SDL_Rect& camera);
  void free();

private:
  Texture m_texture;
  std::vector<SDL_Point> m_positions;

  static const int N_FRAMES = 6;
  std::array<SDL_Point, N_FRAMES> m_positions_clips;

  void calculate_positions_clips();
};

#endif // COIN_HPP
