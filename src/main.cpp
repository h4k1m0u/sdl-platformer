#include "game.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Game game;

/* Used bcos a non-static class method cannot be passed as a C function pointer below */
static void main_loop() {
  game.main_loop();
}

int main() {
  game.init();

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
