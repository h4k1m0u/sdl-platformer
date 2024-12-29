#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL_mixer.h>

#include "arrow_buttons.hpp"
#include "player.hpp"
#include "constants.hpp"
#include "tilemap.hpp"
#include "coins.hpp"
#include "enemies.hpp"
#include "text.hpp"

class Game {
private:
  SDL_Window* m_window;
  SDL_Renderer* m_renderer;

  TTF_Font* m_font;
  Mix_Music* m_music;
  Mix_Chunk* m_sound;

  Tilemap m_tilemap;
  Player m_player;
  ArrowButtons m_arrow_buttons;
  Coins m_coins;
  Enemies m_enemies;
  Text m_text;

  SDL_Rect m_camera = { 0, 0, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT };

  void init_sdl();
  void init_sounds();
  void init_text();

  bool detect_collision_ground();
  void detect_collision_coins(int& score);
  void detect_collision_enemies();

  void render(int frame);
  void handle_events(bool is_on_ground);
  void quit_game();
  void free();

public:
  Game();
  void init();
  void main_loop();
};

#endif
