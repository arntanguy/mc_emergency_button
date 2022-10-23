#include <emergency_button/WiredEmergencyButton.h>
#include <emergency_button/WirelessEmergencyButton.h>
#include <iostream>
#include <signal.h>
#include <thread>

bool loop = true;
void handler(int /*signum*/)
{
  loop = false;
};

int main(int argc, char * argv[])
{
  emergency_button::WiredEmergencyButton wiredButton;
  emergency_button::WirelessEmergencyButton wirelessButton;
  wiredButton.connect();
  wirelessButton.connect("/dev/wireless_emergency_button");

  signal(SIGINT, handler);
  while(loop)
  {
    if(wiredButton.connected())
    {
      std::cout << "Emergency (wired): " << wiredButton.emergency() << std::endl;
    }
    if(wirelessButton.connected())
    {
      std::cout << "Emergency (wireless): " << wirelessButton.emergency() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}
