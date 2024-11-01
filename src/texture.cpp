#include <SDL2/SDL_image.h>

#include "texture.hpp"

/**
 * @param size Size of the frame rendered (not of all texture image)
 */
Texture::Texture(const std::string& path_image, const SDL_Point& size, SDL_Renderer* renderer):
  m_renderer(renderer),
  m_texture(IMG_LoadTexture(m_renderer, path_image.c_str())),
  m_width(size.x),
  m_height(size.y)
{
}

Texture::Texture(TTF_Font* font, const std::string& text, SDL_Renderer* renderer):
  m_renderer(renderer),
  m_texture(NULL)
{
  set_from_text(font, text);
}

void Texture::set_from_text(TTF_Font* font, const std::string& text) {
  if (m_texture != NULL)
    free();

  const SDL_Color COLOR_WHITE = { 0xff, 0xff, 0xff, 0xff };
  SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), COLOR_WHITE);
  m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
  SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
  SDL_FreeSurface(surface);
}

int Texture::get_width() {
  return m_width;
}

int Texture::get_height() {
  return m_height;
}

/**
 * @param position Where to position image's upper-left corner on window
 * @param position_clip Local position of cropped frame's upper-left corner
 */
void Texture::render(const SDL_Point& position, const SDL_Point& position_clip) {
  SDL_Rect rect_src = { position_clip.x, position_clip.y, m_width, m_height };
  SDL_Rect rect_dest = { position.x, position.y, m_width, m_height };
  SDL_RenderCopy(m_renderer, m_texture, &rect_src, &rect_dest);
}

void Texture::free() {
  SDL_DestroyTexture(m_texture);
}
