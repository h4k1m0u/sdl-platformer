#include "timer_cooldown.hpp"

const float TIME_COOLDOWN_MS = 1000.0f;

TimerCooldown::TimerCooldown():
  m_is_active(false),
  m_start_ms(0)
{
}

void TimerCooldown::start() {
  m_is_active = true;
  m_start_ms = SDL_GetTicks64();
}

void TimerCooldown::stop() {
  m_is_active = false;
  m_start_ms = 0;
}

bool TimerCooldown::is_cool() const {
  if (!m_is_active) {
    return true;
  }

  Uint64 time_ms = SDL_GetTicks64() - m_start_ms;
  bool is_time_up = time_ms >= TIME_COOLDOWN_MS;

  return is_time_up;
}
