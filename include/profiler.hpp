#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <SDL2/SDL_stdinc.h>

/* Higher precision */
class Profiler {
public:
  Profiler();
  void start();
  void stop();

private:
  Uint64 counts;
};
#endif
