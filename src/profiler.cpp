#include <SDL2/SDL_timer.h>
#include <iostream>

#include "profiler.hpp"

Profiler::Profiler():
  counts(0)
{
}

void Profiler::start() {
  counts = SDL_GetPerformanceCounter();
}

void Profiler::stop() {
  Uint64 delta_counts = SDL_GetPerformanceCounter() - counts;
  double period = 1.0 / SDL_GetPerformanceFrequency();
  double delta = delta_counts * period;

  std::cout << "Delta: " << delta << "s" << '\n';
}
