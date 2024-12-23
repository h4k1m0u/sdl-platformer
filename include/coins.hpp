#ifndef COINS_HPP
#define COINS_HPP

#include <vector>

#include "texture.hpp"
#include "types.hpp"

class Coins {
public:
  Coins() = default;
  Coins(SDL_Renderer* renderer, const std::vector<SDL_Point>& positions);
  const BboxesMap& get_bboxes() const;
  void render(int frame, const SDL_Rect& camera);
  void destroy(int key);
  void free();

private:
  SDL_Renderer* m_renderer;
  Texture m_texture;
  BboxesMap m_bboxes;

  static const int N_FRAMES = 6;
  Frames<N_FRAMES> m_positions_clips;

  void calculate_positions_clips();
  void calculate_bboxes(const std::vector<SDL_Point>& positions);
};

#endif // COINS_HPP
