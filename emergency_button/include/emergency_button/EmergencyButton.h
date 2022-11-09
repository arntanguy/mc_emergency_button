#pragma once
#include <atomic>
#include <chrono>
#include <mutex>

namespace emergency_button
{

using duration_ms = std::chrono::duration<double, std::milli>;
using duration_us = std::chrono::duration<double, std::micro>;

/** mc_rtc::clock is a clock that is always steady and thus suitable for performance measurements */
using clock = typename std::conditional<std::chrono::high_resolution_clock::is_steady,
                                        std::chrono::high_resolution_clock,
                                        std::chrono::steady_clock>::type;

struct EmergencyButton
{
  inline void required(bool required)
  {
    required_ = required;
  }

  inline bool required() const noexcept
  {
    return required_;
  }

  /**
   * @brief True when the button is in emergency state
   *
   * - When the button is required, we consider that we are in emergency if:
   *   - The button is not connected
   *   - The button is pressed in emergency state
   *
   * - When the button is not required, we consider that we are in emergency
   * only if:
   *  - The button is connected
   *  - The button is in emergency
   *  - If the button is not connected, we are not in emergency
   */
  inline bool emergency() const noexcept
  {
    if(required_)
    {
      return !connected_ || emergency_ || timeSinceLastReceived() > timeout_;
    }
    else
    {
      return connected_ && emergency_;
    }
  }

  /**
   * Connected if we have received new data before timeout and
   * the emergency button device is connected
   **/
  inline bool connected() const noexcept
  {
    return timeSinceLastReceived() < timeout_ && connected_;
  }

  // Timeout in ms
  inline void timeout(double ms)
  {
    timeout_ = ms;
  }

  inline double timeout() const noexcept
  {
    return timeout_;
  }

  // Time since last received in ms
  inline double timeSinceLastReceived() const
  {
    std::lock_guard<std::mutex> lock(timeMutex_);
    return time_since_last_received_.count();
  }

protected:
  bool required_{true}; ///< When true, the button will always be in emergency
                        /// if the button is not plugged in
  std::atomic<bool> emergency_{true};
  std::atomic<bool> connected_{false};
  clock::time_point prev_time_;
  duration_ms time_since_last_received_{0}; // time between two successful reading
  mutable std::mutex timeMutex_;
  double timeout_ = 1000; // timeout in ms
};
} // namespace emergency_button
