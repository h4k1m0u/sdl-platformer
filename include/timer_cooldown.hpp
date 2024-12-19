#ifndef TIMER_COOLDOWN_HPP
#define TIMER_COOLDOWN_HPP

#include <SDL2/SDL_timer.h>

class TimerCooldown {
private:
  bool m_is_active;
  Uint64 m_start_ms;

public:
  TimerCooldown();
  void start();
  void stop();
  bool is_cool() const;
};

#endif
