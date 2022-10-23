#pragma once

#include <emergency_button/EmergencyButton.h>
#include <thread>

namespace emergency_button
{
struct WiredEmergencyButton : public EmergencyButton
{
  WiredEmergencyButton();
  virtual ~WiredEmergencyButton();
  void connect();

protected:
  std::thread th_;
};
} // namespace emergency_button