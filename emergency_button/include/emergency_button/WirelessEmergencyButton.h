#pragma once

#include <chrono>
#include <emergency_button/EmergencyButton.h>
#include <thread>

namespace emergency_button
{

struct WirelessEmergencyButton : public EmergencyButton
{
  WirelessEmergencyButton();
  virtual ~WirelessEmergencyButton();
  void connect(const std::string & serial_port);
  double timeSinceLastReceived();

protected:
  bool running_ = true;
  std::thread th_;
};
} // namespace emergency_button
