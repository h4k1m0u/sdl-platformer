#include <fstream>

#include "tilemap_parser.hpp"
#include "constants.hpp"

using namespace Constants;

const std::string PATH_TILEMAP = "./assets/tilemap.txt";

TilemapParser::TilemapParser()
{
}

void TilemapParser::parse_voids(const std::string& line, int& i_col) {
  TILE_TYPE tile_type;

  while (i_col < m_n_cols && (tile_type = static_cast<TILE_TYPE>(line[i_col])) == TILE_TYPE::VOID) {
    i_col++;
  }
}

void TilemapParser::parse_coins(const std::string& line, int i_row, int& i_col) {
  TILE_TYPE tile_type;

  while (i_col < m_n_cols && (tile_type = static_cast<TILE_TYPE>(line[i_col])) == TILE_TYPE::COIN) {
    SDL_Point position = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };
    m_coins.push_back(position);
    i_col++;
  }
}

/**
 * Count sequence of non-void tiles as a single bbox
 * => less bboxes to check collision against
 */
void TilemapParser::parse_grounds(const std::string& line, int i_row, int& i_col) {
  SDL_Point position_sequence = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };
  int n_tiles_sequence = 0;
  TILE_TYPE tile_type;

  // comma operator evaluates left-to-right, but checks truthness only of last operand
  while (tile_type = static_cast<TILE_TYPE>(line[i_col]), i_col < m_n_cols && GROUNDS.contains(tile_type)) {
    SDL_Point position = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };
    m_tiles[tile_type].push_back(position);
    n_tiles_sequence++;
    i_col++;
  }

  if (n_tiles_sequence > 0) {
    SDL_Rect bbox = { position_sequence.x, position_sequence.y, n_tiles_sequence * WIDTH_TILE, HEIGHT_TILE };
    m_bboxes.push_back(bbox);
  }
}

/* Calculate positions & bboxes of obstacle tiles */
void TilemapParser::parse() {
  std::ifstream f_tilemap(PATH_TILEMAP);
  std::string line;
  int i_row = 0;

  while (std::getline(f_tilemap, line)) {
    m_n_cols = line.size();
    int i_col = 0;

    while (i_col < m_n_cols) {
      parse_voids(line, i_col);
      parse_coins(line, i_row, i_col);
      parse_grounds(line, i_row, i_col);
    }

    i_row++;
  }
}

Tiles TilemapParser::get_tiles() const {
  return m_tiles;
}

std::vector<SDL_Rect> TilemapParser::get_bboxes() const {
  return m_bboxes;
}

std::vector<SDL_Point> TilemapParser::get_coins() const {
  return m_coins;
}
