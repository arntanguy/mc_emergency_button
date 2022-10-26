#include "EmergencyButtonPlugin.h"

#include <mc_control/GlobalPluginMacros.h>
#include <emergency_button/WiredEmergencyButton.h>
#include <emergency_button/WirelessEmergencyButton.h>

namespace mc_plugin
{

EmergencyButtonPlugin::~EmergencyButtonPlugin()
{
  mc_rtc::log::info("EmergencyButtonPlugin destroyed");
}

void EmergencyButtonPlugin::init(mc_control::MCGlobalController & controller, const mc_rtc::Configuration & config)
{
  if(config("type", std::string{"wireless"}) == "wireless")
  {
    auto wirelessButton = std::make_shared<emergency_button::WirelessEmergencyButton>();
    wirelessButton->connect(config("port", std::string{"/dev/ttyUSB0"}));
    emergencyButton_ = wirelessButton;
    mc_rtc::log::info("[EmergencyButtonPlugin] Using wireless button");
  }
  else
  {
    auto wiredButton = std::make_shared<emergency_button::WiredEmergencyButton>();
    wiredButton->connect();
    emergencyButton_ = wiredButton;
  }
  emergencyButton_->required(config("required", true));

  auto & ctl = controller.controller();
  ctl.datastore().make<bool>("EmergencyButtonPlugin", true);
  ctl.datastore().make_call("EmergencyButtonPlugin::Connected",
                            [this]() -> bool { return emergencyButton_->connected(); });
  ctl.datastore().make_call("EmergencyButtonPlugin::State", [this]() -> bool { return emergencyButton_->emergency(); });

  if(config("gui", true))
  {
    ctl.gui()->addElement(this, {"EmergencyButton"},
                          mc_rtc::gui::Label("Connected", [this]() { return emergencyButton_->connected(); }),
                          mc_rtc::gui::Label("Emergency", [this]() { return emergencyButton_->emergency(); }));
  }
  mc_rtc::log::info("EmergencyButtonPlugin::initialized with configuration:\n{}", config.dump(true, true));
}

void EmergencyButtonPlugin::reset(mc_control::MCGlobalController & controller)
{
  mc_rtc::log::info("EmergencyButtonPlugin::reset called");
}

void EmergencyButtonPlugin::before(mc_control::MCGlobalController &) {}

void EmergencyButtonPlugin::after(mc_control::MCGlobalController & controller) {}

mc_control::GlobalPlugin::GlobalPluginConfiguration EmergencyButtonPlugin::configuration()
{
  mc_control::GlobalPlugin::GlobalPluginConfiguration out;
  out.should_run_before = false;
  out.should_run_after = false;
  out.should_always_run = true;
  return out;
}

} // namespace mc_plugin

EXPORT_MC_RTC_PLUGIN("EmergencyButtonPlugin", mc_plugin::EmergencyButtonPlugin)
