#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>

#include "texture.hpp"
#include "types.hpp"

class Tilemap {
public:
  Tilemap() = default;
  Tilemap(SDL_Renderer* renderer);
  void render_to_texture(const Tiles& tiles, const std::vector<SDL_Rect>& bboxes);
  void render(const SDL_Rect& camera);
  void free();

private:
  SDL_Renderer* m_renderer;
  Texture m_texture;
  SDL_Texture* m_texture_raw;
};

#endif
