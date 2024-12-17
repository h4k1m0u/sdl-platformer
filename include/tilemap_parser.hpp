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

  const Tiles& get_tiles_ground() const;
  const std::vector<SDL_Rect>& get_bboxes_ground() const;
  const std::vector<SDL_Point>& get_coins() const;
  const std::vector<PatrolTrajectory>& get_patrol_trajectories() const;

private:
  std::unordered_map<TILE_TYPE, std::vector<SDL_Point>> m_tiles_ground;
  std::vector<SDL_Rect> m_bboxes_ground;
  std::vector<SDL_Point> m_coins;
  std::vector<PatrolTrajectory> m_patrol_trajectories;
  int m_n_cols;

  void parse_voids(const std::string& line, int& i_col);
  void parse_grounds(const std::string& line, int i_row, int& i_col);
  void parse_coin(const std::string& line, int i_row, int& i_col);
  void parse_patrol_point(const std::string& line, int i_row, int& i_col, PatrolTrajectory& trajectory);
};

#endif // TILEMAP_PARSER_HPP
