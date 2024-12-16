#include <iostream>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "arrow_buttons.hpp"
#include "player.hpp"
#include "fps.hpp"
#include "constants.hpp"
#include "tilemap.hpp"
#include "tilemap_parser.hpp"
#include "coins.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// global vars (restructuring required by emscripten)
SDL_Window* window;
SDL_Renderer* renderer;

TTF_Font* font;
Mix_Music* music;
Mix_Chunk* sound;

Tilemap tilemap;
Player player;
FPS fps;
ArrowButtons arrow_buttons;
Coins coins;

SDL_Rect camera = { 0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT };

/* Free all loaded assets */
static void free() {
  Mix_FreeMusic(music);
  Mix_FreeChunk(sound);
  Mix_CloseAudio();

  tilemap.free();
  player.free();
  fps.free();
  arrow_buttons.free();
  coins.free();

  TTF_CloseFont(font);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

static void main_loop() {
  static int frame = 0;
  static int score = 0;

  // physics (collision detection between player & ground tiles)
  SDL_Point point_contact;
  auto [ side_x, side_y ] = player.check_collision_ground(point_contact);
  bool is_on_ground = (side_y == Collision::SideY::ABOVE);

  // collision with coins
  int key_coin;
  std::unordered_map<int, SDL_Rect> bboxes_coins = coins.get_bboxes();
  bool collides_coin = player.check_collision_coins(bboxes_coins, key_coin);

  if (collides_coin) {
    coins.destroy(key_coin);
    score++;
    std::cout << "Collides with coin " << key_coin << " - Score: " << score << '\n';
  }

  // std::cout << frame << " main loop(): " << "on_ground: " << is_on_ground << " point_contact.y: " << point_contact.y << '\n';

  // process even queue once every frame
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
      free();

      #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();  /* this should "kill" the app. */
      #else
        std::exit(0);
      #endif
    }
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
      Mix_PlayChannel(-1, sound, 0);
    }

    else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP ||
             e.type == SDL_FINGERDOWN || e.type == SDL_FINGERUP) {
      arrow_buttons.handle_event(e);
    }
  }

  if (is_on_ground) {
    const Uint8* keys_states = SDL_GetKeyboardState(NULL);
    std::unordered_map<Button, bool> clicked = arrow_buttons.get_clicked();
    player.handle_event(keys_states, clicked);
  } else {
    player.fall();
  }

  // camera follows player (centered around it)
  SDL_Point center_player = player.get_center();
  camera.x = std::clamp(center_player.x - Constants::SCREEN_WIDTH/2, 0, Constants::LEVEL_WIDTH - Constants::SCREEN_WIDTH);
  camera.y = std::clamp(center_player.y - Constants::SCREEN_HEIGHT/2, 0, Constants::LEVEL_HEIGHT - Constants::SCREEN_HEIGHT);

  // std::cout << frame << " camera: " << camera.x << " " << camera.y << '\n';

  // TODO: too many calls to player.check_collision_ground() ???
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
  arrow_buttons.render();
  coins.render(frame, camera);

  SDL_RenderPresent(renderer);
  frame++;
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

  // parse tilemap
  TilemapParser tilemap_parser;
  tilemap_parser.parse();
  std::vector<SDL_Rect> obstacles = tilemap_parser.get_bboxes();
  Tiles tiles = tilemap_parser.get_tiles();
  std::vector<SDL_Point> positions_coins = tilemap_parser.get_coins();

  std::cout << "# of grounds: " << obstacles.size() << '\n';
  /*
  for (const SDL_Rect& bbox : obstacles) {
    std::cout << "Obstacle: " << bbox.x << " " << bbox.y << " "
              << bbox.w << " " << bbox.h << '\n';
  }
  return 0;
  */

  // tilemap
  tilemap = Tilemap(renderer);
  tilemap.render_to_texture(tiles, obstacles);

  // arrow buttons (for mobile)
  arrow_buttons = ArrowButtons(renderer);

  // characters
  player = Player(renderer, obstacles);
  coins = Coins(renderer, positions_coins);

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
  fps.start_timer();

  // game loop
  #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
  #else
    while (true) {
      main_loop();
    }
  #endif

  return 0;
}
