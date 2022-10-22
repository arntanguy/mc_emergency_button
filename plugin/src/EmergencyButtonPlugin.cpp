#include "EmergencyButtonPlugin.h"

#include <mc_control/GlobalPluginMacros.h>

namespace mc_plugin
{

EmergencyButtonPlugin::~EmergencyButtonPlugin() = default;

void EmergencyButtonPlugin::init(mc_control::MCGlobalController & controller, const mc_rtc::Configuration & config)
{
  mc_rtc::log::info("EmergencyButtonPlugin::init called with configuration:\n{}", config.dump(true, true));
}

void EmergencyButtonPlugin::reset(mc_control::MCGlobalController & controller)
{
  mc_rtc::log::info("EmergencyButtonPlugin::reset called");
}

void EmergencyButtonPlugin::before(mc_control::MCGlobalController &)
{
  mc_rtc::log::info("EmergencyButtonPlugin::before");
}

void EmergencyButtonPlugin::after(mc_control::MCGlobalController & controller)
{
  mc_rtc::log::info("EmergencyButtonPlugin::after");
}

mc_control::GlobalPlugin::GlobalPluginConfiguration EmergencyButtonPlugin::configuration()
{
  mc_control::GlobalPlugin::GlobalPluginConfiguration out;
  out.should_run_before = true;
  out.should_run_after = true;
  out.should_always_run = true;
  return out;
}

} // namespace mc_plugin

EXPORT_MC_RTC_PLUGIN("EmergencyButtonPlugin", mc_plugin::EmergencyButtonPlugin)
