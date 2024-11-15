#include <fstream>
#include <iostream>

#include "tilemap.hpp"

Tilemap::Tilemap(SDL_Renderer* renderer):
  m_texture(PATH_TEXTURE, { WIDTH_TILE, HEIGHT_TILE }, renderer)
{
  parse();
}

void Tilemap::parse() {
  std::ifstream f_tilemap(PATH_TILEMAP);
  std::string line;

  while (std::getline(f_tilemap, line)) {
    std::vector<char> row;

    for (size_t i_col = 0; i_col < line.size(); ++i_col) {
      char c = line[i_col];
      row.push_back(c);
    }

    m_map.push_back(row);
  }

  m_n_rows = m_map.size();
  m_n_cols = m_map[0].size();

  std::cout << "nrows: " << m_n_rows << '\n';
  std::cout << "ncols: " << m_n_cols << '\n';
  std::cout << "tilemap[9, 8]: " << m_map[9][8] << '\n';
}

void Tilemap::render() {
  for (int i_row = 0; i_row < m_n_rows; ++i_row) {
    const std::vector<char>& row = m_map[i_row];

    for (int i_col = 0; i_col < m_n_cols; ++i_col) {
      char c = row[i_col];
      TILE_TYPE tile_type = static_cast<TILE_TYPE>(c);

      if (tile_type == TILE_TYPE::VOID)
        continue;

      SDL_Point position_clip = POSITIONS_CLIPS.at(tile_type);
      // std::cout << "position_clip: " << position_clip.x << " " << position_clip.y << '\n';
      SDL_Point position = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };
      m_texture.render(position, position_clip);
    }
  }
}

void Tilemap::free() {
  m_texture.free();
}
