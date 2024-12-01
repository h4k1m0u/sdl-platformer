#include <iostream>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "player.hpp"
#include "fps.hpp"
#include "constants.hpp"
#include "tilemap.hpp"

/*
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
*/

// global vars (restructuring required by emscripten)
SDL_Window* window;
SDL_Renderer* renderer;

TTF_Font* font;
Mix_Music* music;
Mix_Chunk* sound;

Tilemap tilemap;
Player player;
FPS fps;

/* Free all loaded assets */
static void free() {
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
}

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

  window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  // SDL_RENDERER_ACCELERATED: render textures stored on GPU (faster to blit than surfaces stored on CPU memory)
  // SDL_RENDERER_PRESENTVSYNC: sync'ed with screen refresh rate (otherwise fps > 1000)
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // camera
  SDL_Rect camera = { 0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT };

  // tilemap
  tilemap = Tilemap(renderer);
  tilemap.render_to_texture();
  std::vector<SDL_Rect> obstacles = tilemap.get_bboxes();
  std::cout << "# of grounds: " << obstacles.size() << '\n';
  /*
  for (const SDL_Rect& bbox : obstacles) {
    std::cout << "Obstacle: " << bbox.x << " " << bbox.y << " "
              << bbox.w << " " << bbox.h << '\n';
  }
  return 0;
  */

  // player
  player = Player(renderer, obstacles);

  // load font
  const std::string path_font = "/usr/share/fonts/noto/NotoSerif-Regular.ttf";
  const int FONT_SIZE = 24;
  font = TTF_OpenFont(path_font.c_str(), FONT_SIZE);

  // load music & sounds
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

  std::string path_music = "./assets/caketown.mp3";
  music = Mix_LoadMUS(path_music.c_str());
  // Mix_PlayMusic(music, -1);

  std::string path_sound = "./assets/sound.wav";
  sound = Mix_LoadWAV(path_sound.c_str());

  if (music == NULL || sound == NULL) {
    std::cout << "Error loading music file: " << Mix_GetError() << '\n';
    return 1;
  }

  // fps text
  fps = FPS(font, renderer);

  int frame = 0;
  bool quit = false;
  fps.start_timer();

  while (!quit) {
    // physics (collision detection between player & ground tiles)
    SDL_Point point_contact;
    auto [ side_x, side_y ] = player.check_collision(point_contact);
    bool is_on_ground = (side_y == Collision::SideY::ABOVE);

    // std::cout << frame << " main loop(): " << "on_ground: " << is_on_ground << " point_contact.y: " << point_contact.y << '\n';

    // process even queue once every frame
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        quit = true;
      else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
        Mix_PlayChannel(-1, sound, 0);
    }

    if (is_on_ground) {
      const Uint8* keys_states = SDL_GetKeyboardState(NULL);
      player.handle_event(keys_states);
    } else {
      player.fall();
    }

    // camera follows player (centered around it)
    SDL_Point center_player = player.get_center();
    camera.x = std::clamp(center_player.x - Constants::SCREEN_WIDTH/2, 0, Constants::LEVEL_WIDTH - Constants::SCREEN_WIDTH);
    camera.y = std::clamp(center_player.y - Constants::SCREEN_HEIGHT/2, 0, Constants::LEVEL_HEIGHT - Constants::SCREEN_HEIGHT);

    // std::cout << frame << " camera: " << camera.x << " " << camera.y << '\n';

    // TODO: too many calls to player.check_collision() ???
    // TODO: player.move() ???

    // recalculate fps
    if (frame % 10 == 0) {
      fps.calculate(frame);
    }

    // clear window
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer);

    tilemap.render(camera);
    player.render(frame, camera);
    fps.render();

    SDL_RenderPresent(renderer);
    frame++;
  }

  free();

  return 0;
}
