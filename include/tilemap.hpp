#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <unordered_map>
#include <vector>

#include "texture.hpp"
#include "tile_type.hpp"

class Tilemap {
public:
  Tilemap() = default;
  Tilemap(SDL_Renderer* renderer);
  std::vector<SDL_Rect> get_bboxes() const;
  void render_to_texture();
  void render(const SDL_Rect& camera);
  void free();

private:
  SDL_Renderer* m_renderer;
  Texture m_texture;
  SDL_Texture* m_texture_raw;
  std::unordered_map<TILE_TYPE, std::vector<SDL_Point>> m_tiles;

  std::vector<SDL_Rect> m_bboxes;

  void parse();
};

#endif
