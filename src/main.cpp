#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "player.hpp"
#include "fps.hpp"
#include "constants.hpp"
#include "tilemap.hpp"

int main() {
  // SDL2
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    std::cout << "Error init. sdl2: " << SDL_GetError() << '\n';
    return 1;
  }

  // png loading with sdl2-image
  IMG_Init(IMG_INIT_PNG);

  // fonts loadnig with sdl2_ttf
  TTF_Init();

  SDL_Window* window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  // SDL_RENDERER_ACCELERATED: render textures stored on GPU (faster to blit than surfaces stored on CPU memory)
  // SDL_RENDERER_PRESENTVSYNC: sync'ed with screen refresh rate (otherwise fps > 1000)
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // tilemap
  Tilemap tilemap(renderer);
  std::vector<SDL_Rect> obstacles = tilemap.get_bboxes();
  std::cout << "# of grounds: " << obstacles.size() << '\n';

  // player
  Player player(renderer, obstacles);

  // load font
  const std::string path_font = "/usr/share/fonts/noto/NotoSerif-Regular.ttf";
  const int FONT_SIZE = 24;
  TTF_Font* font = TTF_OpenFont(path_font.c_str(), FONT_SIZE);

  // load music & sounds
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

  std::string path_music = "./assets/caketown.mp3";
  Mix_Music* music = Mix_LoadMUS(path_music.c_str());
  // Mix_PlayMusic(music, -1);

  std::string path_sound = "./assets/sound.wav";
  Mix_Chunk* sound = Mix_LoadWAV(path_sound.c_str());

  if (music == NULL || sound == NULL) {
    std::cout << "Error loading music file: " << Mix_GetError() << '\n';
    return 1;
  }

  // fps text
  FPS fps(font, renderer);

  int frame = 0;
  bool quit = false;
  fps.start_timer();

  while (!quit) {
    // physics (collision detection player & ground tiles)
    SDL_Point point_contact;
    bool is_on_ground = player.check_collision(Collision::Side::TOP, point_contact);

    std::cout << frame << " main loop(): " << "on_ground: " << is_on_ground << " point_contact.y: " << point_contact.y << '\n';

    if (!is_on_ground) {
      player.fall();
    }

    // process even queue once every frame
    SDL_Event e;
    bool jump = false;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        quit = true;
      else if (is_on_ground && e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w))
        jump = true;
      else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
        Mix_PlayChannel(-1, sound, 0);
    }

    if (is_on_ground) {
      // prevent player from jumping while in the air
      if (jump) {
        player.jump();
      } else {
        const Uint8* keys_states = SDL_GetKeyboardState(NULL);
        player.handle_event(keys_states);
      }
    }

    // TODO: too many calls to player.check_collision() ???
    // TODO: player.move() ???

    // recalculate fps
    if (frame % 10 == 0) {
      fps.calculate(frame);
    }

    // clear window & draw textures
    const SDL_Color COLOR_SKY = { 80, 255, 232, 255 };
    SDL_SetRenderDrawColor(renderer, COLOR_SKY.r, COLOR_SKY.g, COLOR_SKY.b, COLOR_SKY.a);
    SDL_RenderClear(renderer);

    tilemap.render();
    player.render(frame);
    fps.render();

    SDL_RenderPresent(renderer);
    frame++;
  }

  Mix_FreeMusic(music);
  Mix_FreeChunk(sound);
  Mix_CloseAudio();

  tilemap.free();
  player.free();
  fps.free();

  TTF_CloseFont(font);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
