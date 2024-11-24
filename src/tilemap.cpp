#include <fstream>

#include "tilemap.hpp"
#include "constants.hpp"
#include "drawer.hpp"

Tilemap::Tilemap(SDL_Renderer* renderer):
  m_renderer(renderer),
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
    int i_col = 0;

    while (i_col < n_cols) {
      // ignore void tiles
      TILE_TYPE tile_type = static_cast<TILE_TYPE>(line[i_col]);
      if (tile_type == TILE_TYPE::VOID) {
        i_col++;
        continue;
      }

      // count sequence of non-void tiles as single bbox (less bboxes to check collision against)
      SDL_Point position_sequence = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };
      int n_tiles_sequence = 0;

      while (i_col < n_cols && (tile_type = static_cast<TILE_TYPE>(line[i_col])) != TILE_TYPE::VOID) {
        SDL_Point position = { i_col * WIDTH_TILE, i_row * HEIGHT_TILE };
        m_tiles[tile_type].push_back(position);
        n_tiles_sequence++;
        i_col++;
      }

      SDL_Rect bbox = { position_sequence.x, position_sequence.y, n_tiles_sequence * WIDTH_TILE, HEIGHT_TILE };
      m_bboxes.push_back(bbox);
    }

    i_row++;
  }
}

std::vector<SDL_Rect> Tilemap::get_bboxes() const {
  return m_bboxes;
}

void Tilemap::render(const SDL_Rect& camera) {
  // TODO: add frustum culling to avoid rendering outside camera ?
  for (TILE_TYPE tile_type : { TILE_TYPE::ROCK, TILE_TYPE::GRASS, TILE_TYPE::STONE }) {
    SDL_Point position_clip = POSITIONS_CLIPS.at(tile_type);
    std::vector<SDL_Point> positions = m_tiles[tile_type];

    // change of origin rel. to camera
    for (const SDL_Point& position : positions) {
      SDL_Point position_rel = { position.x - camera.x, position.y - camera.y };
      m_texture.render(position_rel, position_clip);
    }
  }

  // show bboxes in debug mode
  if (Constants::DEBUG) {
    for (const SDL_Rect& bbox : m_bboxes) {
      Drawer::draw_bbox(m_renderer, bbox, camera);
    }
  }
}

void Tilemap::free() {
  m_texture.free();
}
