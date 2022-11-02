#include "EmergencyButtonPlugin.h"

#include <mc_control/GlobalPluginMacros.h>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

namespace mc_plugin
{

EmergencyButtonPlugin::~EmergencyButtonPlugin()
{
  mc_rtc::log::info("EmergencyButtonPlugin destroyed");
}

void EmergencyButtonPlugin::init(mc_control::MCGlobalController & controller, const mc_rtc::Configuration & config)
{
  config("gui", with_gui_);
  reset(controller);
}

void EmergencyButtonPlugin::reset(mc_control::MCGlobalController & controller)
{
  shm_obj_ = bip::shared_memory_object(bip::open_or_create, "emergency_button_shm", bip::read_only);
  shm_region_ = bip::mapped_region(shm_obj_, bip::read_only);
  data_ = reinterpret_cast<EmergencyButtonData *>(shm_region_.get_address());

  auto & ctl = controller.controller();
  ctl.datastore().make<bool>("EmergencyButtonPlugin", true);
  ctl.datastore().make_call("EmergencyButtonPlugin::Connected", [this]() -> bool { return data_->connected; });
  ctl.datastore().make_call("EmergencyButtonPlugin::State", [this]() -> bool { return data_->state; });

  if(with_gui_)
  {
    ctl.gui()->addElement(this, {"EmergencyButton"},
                          mc_rtc::gui::Label("Connected", [this]() { return data_->connected; }),
                          mc_rtc::gui::Label("Emergency", [this]() { return data_->state; }));
  }
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
