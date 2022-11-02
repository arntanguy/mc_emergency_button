/*
 * Copyright 2021 CNRS-UM LIRMM, CNRS-AIST JRL
 */

#pragma once

#include <mc_control/GlobalPlugin.h>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

#include "EmergencyButtonData.h"

namespace bip = boost::interprocess;

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
  bip::shared_memory_object shm_obj_;
  bip::mapped_region shm_region_;
  EmergencyButtonData * data_;
  bool with_gui_ = true;
};

} // namespace mc_plugin
