#pragma once
#include <atomic>

namespace emergency_button
{
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
      return !connected_ || emergency_;
    }
    else
    {
      return connected_ && emergency_;
    }
  }

  inline bool connected() const noexcept
  {
    return connected_;
  }

protected:
  bool required_{true}; ///< When true, the button will always be in emergency
                        /// if the button is not plugged in
  std::atomic<bool> emergency_{true};
  std::atomic<bool> connected_{false};
};
} // namespace emergency_button
