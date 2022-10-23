#pragma once
#include <atomic>

namespace emergency_button
{
struct EmergencyButton
{
  inline bool emergency() const noexcept
  {
    return emergency_;
  }

  inline bool connected() const noexcept
  {
    return connected_;
  }

protected:
  std::atomic<bool> emergency_{true};
  std::atomic<bool> connected_{false};
};
} // namespace emergency_button
