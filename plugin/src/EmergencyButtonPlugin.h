/*
 * Copyright 2021 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#include <mc_control/GlobalPlugin.h>
#include <emergency_button/EmergencyButton.h>

namespace mc_plugin
{

struct EmergencyButtonPlugin : public mc_control::GlobalPlugin
{
  void init(mc_control::MCGlobalController & controller, const mc_rtc::Configuration & config) override;

  void reset(mc_control::MCGlobalController & controller) override;

  void before(mc_control::MCGlobalController &) override;

  void after(mc_control::MCGlobalController & controller) override;

  mc_control::GlobalPlugin::GlobalPluginConfiguration configuration() override;

  ~EmergencyButtonPlugin() override;

private:
  std::shared_ptr<emergency_button::EmergencyButton> emergencyButton_;
};

} // namespace mc_plugin
