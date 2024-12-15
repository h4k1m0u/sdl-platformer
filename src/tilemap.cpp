#include "tilemap.hpp"
#include "constants.hpp"
#include "drawer.hpp"

using namespace Constants;

const std::string PATH_TEXTURE = "./assets/tileset.png";

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
}

/**
 * Rendering to a texture is an order of magnitude quicker than rendering individual tiles each frame.
 * For tilemap: 1μs/frame vs 20μs before. Frustum culling didn't improve much (19μs).
 */
void Tilemap::render_to_texture(const Tiles& tiles, const std::vector<SDL_Rect>& bboxes) {
  // render to texture
  SDL_SetRenderTarget(m_renderer, m_texture_raw);

  // background
  const SDL_Color COLOR_SKY = { 80, 255, 232, 255 };
  SDL_SetRenderDrawColor(m_renderer, COLOR_SKY.r, COLOR_SKY.g, COLOR_SKY.b, COLOR_SKY.a);
  SDL_RenderClear(m_renderer);

  // tiles
  for (TILE_TYPE tile_type : GROUNDS) {
    if (!tiles.contains(tile_type))
      continue;

    SDL_Point position_clip = POSITIONS_CLIPS.at(tile_type);
    std::vector<SDL_Point> positions = tiles.at(tile_type);

    for (const SDL_Point& position : positions) {
      m_texture.render(position, position_clip);
    }
  }

  // show bboxes in debug mode
  if (Constants::DEBUG) {
    for (const SDL_Rect& bbox : bboxes) {
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
