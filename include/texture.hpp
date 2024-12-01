#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL2/SDL_ttf.h>
#include <string>

/* Renders textures stored on GPU (faster to blit than surfaces stored on CPU memory) */
class Texture {
public:
  Texture() = default;
  Texture(const std::string& path_image, const SDL_Point& size, SDL_Renderer* renderer);
  Texture(TTF_Font* font, const std::string& text, SDL_Renderer* renderer);

  int get_width() const;
  int get_height() const;

  void set_from_text(TTF_Font* font, const std::string& text);

  void render(const SDL_Point& position, const SDL_Point& position_clip={ 0, 0 });
  void free();

private:
  SDL_Renderer* m_renderer;
  SDL_Texture* m_texture;

  int m_width;
  int m_height;
};

#endif
