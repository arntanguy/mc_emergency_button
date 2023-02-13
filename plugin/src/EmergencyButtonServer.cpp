#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

#include <mc_rtc/Configuration.h>

#include <emergency_button/EmergencyButton.h>
#include <emergency_button/WiredEmergencyButton.h>
#include <emergency_button/WirelessEmergencyButton.h>

#include "EmergencyButtonData.h"

namespace bip = boost::interprocess;

int main(int argc, char * argv[])
{
  if(argc < 2)
  {
    mc_rtc::log::error("[usage] {} [config]", argv[0]);
    return 1;
  }
  auto config = mc_rtc::Configuration(argv[1]);
  std::shared_ptr<emergency_button::EmergencyButton> button;
  if(config("type", std::string{"wireless"}) == "wireless")
  {
    std::string port = config("port", std::string("/dev/ttyUSB0"));
    mc_rtc::log::info("[EmergencyButtonServer] Connected to wireless button via {}", port);
    auto wButton = std::make_shared<emergency_button::WirelessEmergencyButton>();
    wButton->connect(port);
    button = wButton;
  }
  else
  {
    mc_rtc::log::info("[EmergencyButtonServer] Connected to wired button");
    auto wButton = std::make_shared<emergency_button::WiredEmergencyButton>();
    wButton->connect();
    button = wButton;
  }
  button->required(config("required", true));
  std::this_thread::sleep_for(std::chrono::seconds(1));
  mc_rtc::log::info("Started");

  bip::shared_memory_object shm_obj(bip::open_or_create, "emergency_button_shm", bip::read_write);
  shm_obj.truncate(sizeof(EmergencyButtonData));

  bip::mapped_region region(shm_obj, bip::read_write);

  EmergencyButtonData data;
  data.connected = button->connected();
  data.state = button->emergency();
  std::memcpy(region.get_address(), &data, sizeof(EmergencyButtonData));

  mc_rtc::log::info("Check connection status");
  size_t i = 0;
  bool was_not_connected = !data.connected;
  bool was_emergency = data.state;
  while(true)
  {
    data.connected = button->connected();
    data.state = button->emergency();
    if(i % 100 == 0)
    {
      mc_rtc::log::info("connected: {}, emergency: {}", data.connected, data.state);
    }
    auto check_state = [](bool current, bool & was, const char * bad_msg, const char * good_msg) {
      if(current == was)
      {
        return;
      }
      was = current;
      if(current)
      {
        mc_rtc::log::critical(bad_msg);
      }
      else
      {
        mc_rtc::log::success(good_msg);
      }
    };
    check_state(!data.connected, was_not_connected, "Lost connection", "Recovered connection");
    check_state(data.state, was_emergency, "Emergency triggered", "Emergency off");
    std::memcpy(region.get_address(), &data, sizeof(EmergencyButtonData));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    i++;
  }

  mc_rtc::log::critical("Connection to the emergency button lost");
  data.connected = false;
  data.state = true;
  std::memcpy(region.get_address(), &data, sizeof(EmergencyButtonData));

  return 0;
}
