#include <SDL2/SDL_image.h>

#include "texture.hpp"

Texture::Texture(const std::string& path_image, SDL_Renderer* renderer):
  m_renderer(renderer),
  m_texture(IMG_LoadTexture(m_renderer, path_image.c_str()))
{
  SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
}

Texture::Texture(TTF_Font* font, const std::string& text, SDL_Renderer* renderer):
  m_renderer(renderer),
  m_texture(NULL)
{
  set_from_text(font, text);
  SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
}

void Texture::set_from_text(TTF_Font* font, const std::string& text) {
  if (m_texture != NULL)
    free();

  const SDL_Color COLOR_WHITE = { 0xff, 0xff, 0xff, 0xff };
  SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), COLOR_WHITE);
  m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
  SDL_FreeSurface(surface);
}

int Texture::get_width() {
  return m_width;
}

int Texture::get_height() {
  return m_height;
}

/**
 * @param position Upper-left corner of image
 */
void Texture::render(const SDL_Point& position) {
  SDL_Rect rect_dest = { position.x, position.y, m_width, m_height };
  SDL_RenderCopy(m_renderer, m_texture, NULL, &rect_dest);
}

void Texture::free() {
  SDL_DestroyTexture(m_texture);
}
