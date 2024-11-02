#include "fps.hpp"
#include "constants.hpp"

/**
 * @param position Position of texture's center
 */
FPS::FPS(TTF_Font* font, SDL_Renderer* renderer):
  m_font(font),
  m_texture(font, "FPS: ", renderer),
  m_start_ticks(0)
{
}

/* Used to ignore time to init SDL & load assets when calculating fps */
void FPS::start_timer() {
  m_start_ticks = SDL_GetTicks64();
}

/* Calculate average fps (less fluctuation then for single frame) */
void FPS::calculate(int frame) {
    Uint64 ticks_ms = SDL_GetTicks64() - m_start_ticks;
    float fps = frame / (ticks_ms / 1000.f);

    const std::string text = "FPS: " + std::to_string(fps);
    m_texture.set_from_text(m_font, text);
}

void FPS::render() {
    SDL_Point position_text = {
      Constants::SCREEN_WIDTH / 2 - m_texture.get_width() / 2,
      Constants::SCREEN_HEIGHT / 2 - m_texture.get_height() / 2
    };
    m_texture.render(position_text);
}

void FPS::free() {
  m_texture.free();
}
