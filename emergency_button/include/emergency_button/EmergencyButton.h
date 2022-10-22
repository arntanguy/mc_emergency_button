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

protected:
  std::atomic<bool> emergency_{true};
};
} // namespace emergency_button
