#ifndef TILEMAP_PARSER_HPP
#define TILEMAP_PARSER_HPP

#include <SDL2/SDL_rect.h>
#include <unordered_map>
#include <vector>
#include <string>

#include "tile_type.hpp"
#include "types.hpp"

class TilemapParser {
public:
  TilemapParser();
  void parse();

  Tiles get_tiles() const;
  std::vector<SDL_Rect> get_bboxes() const;
  std::vector<SDL_Point> get_coins() const;

private:
  std::unordered_map<TILE_TYPE, std::vector<SDL_Point>> m_tiles;
  std::vector<SDL_Rect> m_bboxes;
  std::vector<SDL_Point> m_coins;
  int m_n_cols;

  void parse_voids(const std::string& line, int& i_col);
  void parse_coins(const std::string& line, int i_row, int& i_col);
  void parse_grounds(const std::string& line, int i_row, int& i_col);
};

#endif // TILEMAP_PARSER_HPP
