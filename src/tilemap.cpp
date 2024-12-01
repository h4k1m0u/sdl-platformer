#include <fstream>

#include "tilemap.hpp"
#include "constants.hpp"
#include "drawer.hpp"

const std::string PATH_TILEMAP = "./assets/tilemap.txt";
const std::string PATH_TEXTURE = "./assets/tileset.png";
const int WIDTH_TILE = 32;
const int HEIGHT_TILE = 32;

const std::unordered_map<TILE_TYPE, SDL_Point> POSITIONS_CLIPS = {
  { TILE_TYPE::ROCK,  { 32, 0 } },
  { TILE_TYPE::GRASS, { 64, 0 } },
  { TILE_TYPE::STONE, { 96, 0 } }
};

Tilemap::Tilemap(SDL_Renderer* renderer):
  m_renderer(renderer),
  m_texture(PATH_TEXTURE, { WIDTH_TILE, HEIGHT_TILE }, renderer),
  m_texture_raw(SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, Constants::LEVEL_WIDTH, Constants::LEVEL_HEIGHT))
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

/**
 * Rendering to a texture is an order of magnitude quicker than rendering individual tiles each frame.
 * For tilemap: 1μs/frame vs 20μs before. Frustum culling didn't improve much (19μs).
 */
void Tilemap::render_to_texture() {
  // render to texture
  SDL_SetRenderTarget(m_renderer, m_texture_raw);

  // background
  const SDL_Color COLOR_SKY = { 80, 255, 232, 255 };
  SDL_SetRenderDrawColor(m_renderer, COLOR_SKY.r, COLOR_SKY.g, COLOR_SKY.b, COLOR_SKY.a);
  SDL_RenderClear(m_renderer);

  // tiles
  for (TILE_TYPE tile_type : { TILE_TYPE::ROCK, TILE_TYPE::GRASS, TILE_TYPE::STONE }) {
    SDL_Point position_clip = POSITIONS_CLIPS.at(tile_type);
    std::vector<SDL_Point> positions = m_tiles[tile_type];

    for (const SDL_Point& position : positions) {
      m_texture.render(position, position_clip);
    }
  }

  // show bboxes in debug mode
  if (Constants::DEBUG) {
    for (const SDL_Rect& bbox : m_bboxes) {
      Drawer::draw_bbox(m_renderer, bbox);
    }
  }

  // render to window again
  SDL_SetRenderTarget(m_renderer, NULL);
}

void Tilemap::render(const SDL_Rect& camera) {
  const SDL_Rect rect_dest = { 0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT };
  SDL_RenderCopy(m_renderer, m_texture_raw, &camera, &rect_dest);
}

void Tilemap::free() {
  m_texture.free();
  SDL_DestroyTexture(m_texture_raw);
}
