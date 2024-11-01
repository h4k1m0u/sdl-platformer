#include <iostream>
#include <SDL2/SDL_image.h>

#include "player.hpp"

int main() {
  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;

  // SDL2
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Error init. sdl2: " << SDL_GetError() << '\n';
    return 1;
  }

  // png loading with sdl2-image
  IMG_Init(IMG_INIT_PNG);

  // fonts loadnig with sdl2_ttf
  TTF_Init();

  SDL_Window* window = SDL_CreateWindow("Window title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  // used to render textures stored on GPU (faster to blit than surfaces stored on CPU memory)
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);

  // player
  Player player(renderer);

  // load font
  const std::string path_font = "/usr/share/fonts/noto/NotoSerif-Regular.ttf";
  const int FONT_SIZE = 24;
  TTF_Font* font = TTF_OpenFont(path_font.c_str(), FONT_SIZE);

  // text texture
  unsigned int score = 0;
  const std::string text = "Score: " + std::to_string(score);
  Texture texture_text(font, text, renderer);

  /*
  // load image on surface (on cpu memory)
  std::string path_image = "./car.png";
  SDL_Surface* screen_surface = SDL_GetWindowSurface(window);
  // SDL_Surface* image_surface = SDL_LoadBMP(path_image.c_str());
  SDL_Surface* image_surface = IMG_Load(path_image.c_str());
  SDL_BlitSurface(image_surface, NULL, screen_surface, NULL);
  SDL_UpdateWindowSurface(window);
  */

  /*
  // color surface
  SDL_FillRect(screen_surface, NULL, SDL_MapRGBA(screen_surface->format, 0xff, 0, 0, 0xff));
  SDL_UpdateWindowSurface(window);
  */

  int frame = 0;
  bool quit = false;

  while (!quit) {
    // process even queue once every frame
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;
      else
        player.handle_event(&e);

      /*
      score++;
      const std::string text = "Score: " + std::to_string(score);
      texture_text.set_from_text(font, text);
      */
    }

    // clear window & draw textures
    SDL_RenderClear(renderer);

    player.render(frame);

    SDL_Point position_text = {
      SCREEN_WIDTH / 2 - texture_text.get_width() / 2,
      SCREEN_HEIGHT / 2 - texture_text.get_height() / 2
    };
    texture_text.render(position_text);

    SDL_RenderPresent(renderer);
    frame++;
  }

  // SDL_FreeSurface(image_surface);

  player.free();
  texture_text.free();

  TTF_CloseFont(font);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
