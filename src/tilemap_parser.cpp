#include <fstream>
#include <utility>

#include "tilemap_parser.hpp"
#include "constants.hpp"
#include "sdl_utils.hpp"

using namespace Constants;
using namespace SDLUtils;

const std::string PATH_TILEMAP = "./assets/tilemap.txt";
const SDL_Point UNSET_POINT = { -1, -1 };

TilemapParser::TilemapParser()
{
}

void TilemapParser::parse_voids(const std::string& line, int& i_col) {
  TILE_TYPE tile_type;

  while (i_col < m_n_cols && (tile_type = static_cast<TILE_TYPE>(line[i_col])) == TILE_TYPE::VOID) {
    i_col++;
  }
}

void TilemapParser::parse_coin(const std::string& line, int i_row, int& i_col) {
  if (i_col >= m_n_cols)
    return;

  TILE_TYPE tile_type = static_cast<TILE_TYPE>(line[i_col]);

  if (tile_type == TILE_TYPE::COIN) {
    SDL_Point position = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };
    m_coins.push_back(position);
    i_col++;
  }
}

/**
 * Patrol points exist as a pair & coins can exist in between them
 * => keep track of first inserted point
 */
void TilemapParser::parse_patrol_point(const std::string& line, int i_row, int& i_col, PatrolTrajectory& trajectory) {
  if (i_col >= m_n_cols)
    return;

  TILE_TYPE tile_type = static_cast<TILE_TYPE>(line[i_col]);

  if (tile_type == TILE_TYPE::PATROL_POINT) {
    SDL_Point position = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };

    if (trajectory.first == UNSET_POINT) {
      trajectory.first = position;
    }
    else if (trajectory.second == UNSET_POINT) {
      trajectory.second = position;
      m_patrol_trajectories.push_back(trajectory);
      trajectory = std::make_pair(UNSET_POINT, UNSET_POINT);
    }

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
    m_tiles_ground[tile_type].push_back(position);
    n_tiles_sequence++;
    i_col++;
  }

  if (n_tiles_sequence > 0) {
    SDL_Rect bbox = { position_sequence.x, position_sequence.y, n_tiles_sequence * WIDTH_TILE, HEIGHT_TILE };
    m_bboxes_ground.push_back(bbox);
  }
}

/* Calculate positions & bboxes of obstacle tiles */
void TilemapParser::parse() {
  std::ifstream f_tilemap(PATH_TILEMAP);
  std::string line;
  int i_row = 0;
  PatrolTrajectory trajectory(UNSET_POINT, UNSET_POINT);

  while (std::getline(f_tilemap, line)) {
    m_n_cols = line.size();
    int i_col = 0;

    while (i_col < m_n_cols) {
      parse_voids(line, i_col);
      parse_coin(line, i_row, i_col);
      parse_grounds(line, i_row, i_col);
      parse_patrol_point(line, i_row, i_col, trajectory);
    }

    i_row++;
  }
}

const Tiles& TilemapParser::get_tiles_ground() const {
  return m_tiles_ground;
}

const std::vector<SDL_Rect>& TilemapParser::get_bboxes_ground() const {
  return m_bboxes_ground;
}

const std::vector<SDL_Point>& TilemapParser::get_coins() const {
  return m_coins;
}

const std::vector<PatrolTrajectory>& TilemapParser::get_patrol_trajectories() const {
  return m_patrol_trajectories;
}
