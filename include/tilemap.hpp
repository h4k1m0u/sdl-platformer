#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <unordered_map>
#include <string>
#include <vector>

#include "texture.hpp"

class Tilemap {
public:
  Tilemap(SDL_Renderer* renderer);
  void render();
  void free();

private:
  const std::string PATH_TILEMAP = "./assets/tilemap.txt";
  const std::string PATH_TEXTURE = "./assets/tileset.png";
  const int WIDTH_TILE = 32;
  const int HEIGHT_TILE = 32;

  enum TILE_TYPE : char {
    ROCK = 'R',
    GRASS = 'G',
    STONE = 'S',
    VOID = ' ',
  };

  const std::unordered_map<TILE_TYPE, SDL_Point> POSITIONS_CLIPS = {
    { ROCK,  { 32, 0 } },
    { GRASS, { 64, 0 } },
    { STONE, { 96, 0 } }
  };

  Texture m_texture;
  std::vector<std::vector<char>> m_map;
  int m_n_rows;
  int m_n_cols;

  void parse();
};

#endif
