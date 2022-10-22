#include <emergency_button/WiredEmergencyButton.h>
#include <iostream>
#include <thread>

int main(int argc, char * argv[])
{
  emergency_button::WiredEmergencyButton b;
  b.connect();

  while(true)
  {
    std::cout << "Emergency: " << b.emergency() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}
