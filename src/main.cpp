#include <iostream>
#include <SDL2/SDL_image.h>

#include "player.hpp"
#include "fps.hpp"

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

  // SDL_RENDERER_ACCELERATED: render textures stored on GPU (faster to blit than surfaces stored on CPU memory)
  // SDL_RENDERER_PRESENTVSYNC: sync'ed with screen refresh rate (otherwise fps > 1000)
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);

  // player
  Player player(renderer);

  // load font
  const std::string path_font = "/usr/share/fonts/noto/NotoSerif-Regular.ttf";
  const int FONT_SIZE = 24;
  TTF_Font* font = TTF_OpenFont(path_font.c_str(), FONT_SIZE);

  // fps text
  SDL_Point position_text = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
  FPS fps(position_text, font, renderer);

  int frame = 0;
  bool quit = false;
  fps.start_timer();

  while (!quit) {
    // process even queue once every frame
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;
      else
        player.handle_event(&e);
    }

    // recalculate fps
    if (frame % 10 == 0) {
      fps.calculate(frame);
    }

    // clear window & draw textures
    SDL_RenderClear(renderer);

    player.render(frame);
    fps.render();

    SDL_RenderPresent(renderer);
    frame++;
  }

  player.free();
  fps.free();

  TTF_CloseFont(font);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
