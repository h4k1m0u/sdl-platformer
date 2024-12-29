#include <iostream>
#include <algorithm>
#include <SDL2/SDL_image.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "game.hpp"
#include "tilemap_parser.hpp"

Game::Game():
  // direct initialization for an aggregate
  m_camera({ 0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT })
{
}

void Game::init() {
  init_sdl();

  m_window = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  // SDL_RENDERER_ACCELERATED: render textures stored on GPU (faster to blit than surfaces stored on CPU memory)
  // SDL_RENDERER_PRESENTVSYNC: sync'ed with screen refresh rate (otherwise fps > 1000)
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // arrow buttons (for mobile)
  m_arrow_buttons = ArrowButtons(m_renderer);

  // parse tilemap
  TilemapParser tilemap_parser;
  tilemap_parser.parse();
  const std::vector<SDL_Rect>& bboxes_ground = tilemap_parser.get_bboxes_ground();
  const Tiles& tiles_ground = tilemap_parser.get_tiles_ground();
  const std::vector<SDL_Point>& positions_coins = tilemap_parser.get_coins();
  const std::vector<PatrolTrajectory>& patrol_trajectories = tilemap_parser.get_patrol_trajectories();
  std::cout << "# of grounds: " << bboxes_ground.size() << '\n';

  // tilemap
  m_tilemap = Tilemap(m_renderer);
  m_tilemap.render_to_texture(tiles_ground, bboxes_ground);

  // characters
  m_player = Player(m_renderer, bboxes_ground);
  m_coins = Coins(m_renderer, positions_coins);
  m_enemies = Enemies(m_renderer, patrol_trajectories);

  init_text();
  init_sounds();
}

void Game::init_sdl() {
  // SDL2
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    std::cout << "Error init. sdl2: " << SDL_GetError() << '\n';
    std::exit(1);
  }

  // png loading with sdl2-image
  IMG_Init(IMG_INIT_PNG);

  // fonts loading with sdl2_ttf
  TTF_Init();
}

void Game::init_sounds() {
  // load music & sounds
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

  std::string path_music = "./assets/caketown.mp3";
  m_music = Mix_LoadMUS(path_music.c_str());
  // Mix_PlayMusic(m_music, -1);

  std::string path_sound = "./assets/sound.wav";
  m_sound = Mix_LoadWAV(path_sound.c_str());

  if (m_music == NULL || m_sound == NULL) {
    std::cout << "Error loading music file: " << Mix_GetError() << '\n';
    std::exit(1);
  }
}

void Game::init_text() {
  // load font used to show score
  const std::string path_font = "./assets/NotoSerif-Regular.ttf";
  const int FONT_SIZE = 24;
  m_font = TTF_OpenFont(path_font.c_str(), FONT_SIZE);
  m_text = Text(m_font, m_renderer);
}

/**
 * @param frame Used to keep track of frame to show (for animated sprites)
 */
void Game::render(int frame) {
  // clear window
  SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(m_renderer);

  m_tilemap.render(m_camera);
  m_player.render(frame, m_camera);
  m_arrow_buttons.render();
  m_coins.render(frame, m_camera);
  m_enemies.render(frame, m_camera);
  m_text.render();

  SDL_RenderPresent(m_renderer);
}

/* Process even queue once every frame */
void Game::handle_events(bool is_on_ground) {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
      quit_game();
    }
    else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
      Mix_PlayChannel(-1, m_sound, 0);
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP ||
             e.type == SDL_FINGERDOWN || e.type == SDL_FINGERUP) {
      m_arrow_buttons.handle_events(e);
    }
  }

  if (is_on_ground) {
    const Uint8* keys_states = SDL_GetKeyboardState(NULL);
    const std::unordered_map<Button, bool>& clicked = m_arrow_buttons.get_clicked();
    m_player.handle_events(keys_states, clicked);
  } else {
    m_player.fall();
  }

  // camera follows player (centered around it)
  SDL_Point center_player = m_player.get_center();
  m_camera.x = std::clamp(center_player.x - Constants::SCREEN_WIDTH/2, 0, Constants::LEVEL_WIDTH - Constants::SCREEN_WIDTH);
  m_camera.y = std::clamp(center_player.y - Constants::SCREEN_HEIGHT/2, 0, Constants::LEVEL_HEIGHT - Constants::SCREEN_HEIGHT);
}

/* Detect collision between player & ground tiles */
bool Game::detect_collision_ground() {
  SDL_Point point_contact;
  auto [ side_x, side_y ] = m_player.check_collision_ground(point_contact);
  bool is_on_ground = (side_y == Collision::SideY::ABOVE);
  return is_on_ground;
}

/* Detect collision with coins */
void Game::detect_collision_coins(int& score) {
  int key_coin;
  const BboxesMap& bboxes_coins = m_coins.get_bboxes();
  bool collides_coin = m_player.check_collision_entities(bboxes_coins, key_coin);

  if (collides_coin) {
    m_coins.destroy(key_coin);
    score++;
    m_text.set_score(score);
    std::cout << "Collides with coin " << key_coin << " - Score: " << score << '\n';
  }
}

/* Detect collision with enemies */
void Game::detect_collision_enemies() {
  int key_enemy;
  const BboxesMap& bboxes_enemies = m_enemies.get_bboxes();
  const std::unordered_map<int, TimerCooldown>& timers_enemies = m_enemies.get_timers();
  bool kill_enemy;
  bool collides_enemy = m_player.check_collision_enemies(bboxes_enemies, timers_enemies, key_enemy, kill_enemy);
  if (!collides_enemy)
    return;

  std::cout << "Collides with enemy " << key_enemy << " kill_enemy: " << kill_enemy << " lives: " << m_player.get_n_lives() << '\n';

  m_enemies.stop_cooldown_timer(key_enemy);

  if (kill_enemy) {
    m_enemies.destroy(key_enemy);
  } else {
    int n_lives = m_player.get_n_lives();
    m_text.set_lives(n_lives);
    m_enemies.start_cooldown_timer(key_enemy);
  }
}

void Game::main_loop() {
  static int frame = 0;
  static int score = 0;

  if (!m_player.is_alive()) {
    quit_game();
  }

  /********** Physics **********/
  bool is_on_ground = detect_collision_ground();
  detect_collision_coins(score);
  detect_collision_enemies();

  /********** Input events **********/
  handle_events(is_on_ground);
  // TODO: too many calls to player.check_collision_ground() ???

  /********** Rendering **********/
  render(frame);
  frame++;
}

void Game::quit_game() {
  free();

  #ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();  /* this should "kill" the app. */
  #else
    std::exit(0);
  #endif
}

/* Free all loaded assets */
void Game::free() {
  Mix_FreeMusic(m_music);
  Mix_FreeChunk(m_sound);
  Mix_CloseAudio();

  m_tilemap.free();
  m_player.free();
  m_arrow_buttons.free();
  m_coins.free();
  m_enemies.free();
  m_text.free();

  TTF_CloseFont(m_font);

  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);

  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}
