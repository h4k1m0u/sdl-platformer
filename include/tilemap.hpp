#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <unordered_map>
#include <string>
#include <vector>

#include "texture.hpp"

class Tilemap {
public:
  Tilemap(SDL_Renderer* renderer);
  std::vector<SDL_Rect> get_bboxes() const;
  void render_to_texture();
  void render(const SDL_Rect& camera);
  void free();

private:
  const std::string PATH_TILEMAP = "./assets/tilemap.txt";
  const std::string PATH_TEXTURE = "./assets/tileset.png";
  const int WIDTH_TILE = 32;
  const int HEIGHT_TILE = 32;

  enum class TILE_TYPE : char {
    ROCK = 'R',
    GRASS = 'G',
    STONE = 'S',
    VOID = ' ',
  };

  const std::unordered_map<TILE_TYPE, SDL_Point> POSITIONS_CLIPS = {
    { TILE_TYPE::ROCK,  { 32, 0 } },
    { TILE_TYPE::GRASS, { 64, 0 } },
    { TILE_TYPE::STONE, { 96, 0 } }
  };

  SDL_Renderer* m_renderer;
  Texture m_texture;
  SDL_Texture* m_texture_raw;
  std::unordered_map<TILE_TYPE, std::vector<SDL_Point>> m_tiles;

  std::vector<SDL_Rect> m_bboxes;

  void parse();
};

#endif
