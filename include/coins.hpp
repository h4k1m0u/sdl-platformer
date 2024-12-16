#ifndef COINS_HPP
#define COINS_HPP

#include <array>
#include <unordered_map>
#include <vector>

#include "texture.hpp"

class Coins {
public:
  Coins() = default;
  Coins(SDL_Renderer* renderer, const std::vector<SDL_Point>& positions);
  std::unordered_map<int, SDL_Rect> get_bboxes() const;
  void render(int frame, const SDL_Rect& camera);
  void destroy(int i);
  void free();

private:
  Texture m_texture;
  std::unordered_map<int, SDL_Rect> m_bboxes;

  static const int N_FRAMES = 6;
  std::array<SDL_Point, N_FRAMES> m_positions_clips;

  void calculate_positions_clips();
  void calculate_bboxes(const std::vector<SDL_Point>& positions);
};

#endif // COINS_HPP
