#include "coins.hpp"
#include "drawer.hpp"
#include "constants.hpp"

const std::string PATH_TEXTURE = "./assets/coin.png";

const int WIDTH_COIN = 18;
const int HEIGHT_COIN = 20;

Coins::Coins(SDL_Renderer* renderer, const std::vector<SDL_Point>& positions):
  m_renderer(renderer),
  m_texture(PATH_TEXTURE, { WIDTH_COIN, HEIGHT_COIN }, renderer)
{
  calculate_bboxes(positions);
  calculate_positions_clips();
}

void Coins::calculate_bboxes(const std::vector<SDL_Point>& positions) {
  for (size_t key = 0; key < positions.size(); ++key) {
    SDL_Point position = positions[key];
    m_bboxes[key] = { position.x, position.y, WIDTH_COIN, HEIGHT_COIN };
  }
}

void Coins::calculate_positions_clips() {
  for (int i = 0; i < N_FRAMES; ++i) {
    m_positions_clips[i] = { i * WIDTH_COIN, 0 };
  }
}

const BboxesMap& Coins::get_bboxes() const {
  return m_bboxes;
}

/* hashmaps have constant-time complexity for removal (linear for vectors) */
void Coins::destroy(int key) {
  m_bboxes.erase(key);
}

void Coins::render(int frame, const SDL_Rect& camera) {
  // slow down animation (change image every 6 frames)
  int frame_coin = (frame / N_FRAMES) % N_FRAMES;
  SDL_Point position_clip = m_positions_clips[frame_coin];

  // change of origin rel. to camera
  for (const auto& [ key, bbox ] : m_bboxes) {
    SDL_Point position_rel = { bbox.x - camera.x, bbox.y - camera.y };
    m_texture.render(position_rel, position_clip);

    // show bbox in debug mode
    if (Constants::DEBUG) {
      Drawer::draw_bbox(m_renderer, bbox, &camera);
    }
  }
}

void Coins::free() {
  m_texture.free();
}
