#include <fstream>

#include "tilemap.hpp"

Tilemap::Tilemap(SDL_Renderer* renderer):
  m_texture(PATH_TEXTURE, { WIDTH_TILE, HEIGHT_TILE }, renderer)
{
  parse();
}

/* Calculate positions & bboxes of obstacle tiles */
void Tilemap::parse() {
  std::ifstream f_tilemap(PATH_TILEMAP);
  std::string line;
  int i_row = 0;

  while (std::getline(f_tilemap, line)) {
    int n_cols = line.size();

    for (int i_col = 0; i_col < n_cols; ++i_col) {
      char c = line[i_col];
      TILE_TYPE tile_type = static_cast<TILE_TYPE>(c);
      if (tile_type == TILE_TYPE::VOID)
        continue;

      SDL_Point position = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };
      m_tiles[tile_type].push_back(position);

      SDL_Rect bbox = { position.x, position.y, WIDTH_TILE, HEIGHT_TILE };
      m_bboxes.push_back(bbox);
    }

    i_row++;
  }
}

std::vector<SDL_Rect> Tilemap::get_bboxes() const {
  return m_bboxes;
}

void Tilemap::render() {
  for (TILE_TYPE tile_type : { TILE_TYPE::ROCK, TILE_TYPE::GRASS, TILE_TYPE::STONE }) {
    SDL_Point position_clip = POSITIONS_CLIPS.at(tile_type);
    std::vector<SDL_Point> positions = m_tiles[tile_type];

    for (const SDL_Point& position : positions) {
      m_texture.render(position, position_clip);
    }
  }
}

void Tilemap::free() {
  m_texture.free();
}
