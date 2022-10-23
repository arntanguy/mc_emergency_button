#include <boost/filesystem.hpp>
#include <emergency_button/WirelessEmergencyButton.h>
#include <iostream>

extern "C"
{
#include "usbio.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
}

namespace emergency_button
{

WirelessEmergencyButton::WirelessEmergencyButton() {}

WirelessEmergencyButton::~WirelessEmergencyButton()
{
  connected_ = false;
  if(th_.joinable())
  {
    th_.join();
  }
}

void WirelessEmergencyButton::connect(const std::string & serial_port_)
{

  th_ = std::thread([serial_port_, this]() {
    std::cout << "[wireless] Looking wireless button " << serial_port_ << " ..." << std::endl;
    auto serial_port = std::string{};
    try
    {
      serial_port = boost::filesystem::canonical(serial_port_).string();
    }
    catch(...)
    {
      connected_ = false;
      std::cerr << "[wireless] Serial port " << serial_port_ << " is neither a device handle nor a symbolic link"
                << std::endl;
      return;
    }

    // Try to find wireless button
    int fd = open(serial_port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(fd < 0)
    {
      connected_ = false;
      close(fd);
      std::cerr << "[wireless] No wireless button found" << std::endl;
      return;
    }
    else
    {
      std::cout << "[wireless] Found wireless emergency button: " << serial_port << std::endl;
      connected_ = true;
    }

    fcntl(fd, F_SETFL, 0);
    const int baudRate = B57600;
    struct termios tio;
    tcgetattr(fd, &tio);
#if 0
    tio.c_cflag += CREAD;
    tio.c_cflag += CLOCAL;
    tio.c_cflag += CS8;
    tio.c_cflag += 0;
    tio.c_cflag += 0;
#endif
    cfsetispeed(&tio, baudRate);
    cfsetospeed(&tio, baudRate);
    // cfmakeraw(&tio);
    // non canonical, non echo back
    tio.c_lflag &= ~(ECHO | ICANON);
    // non blocking
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSANOW, &tio);
    // ioctl(fd, TCSETS, &tio);

    bool button_prev = true;
    int c = 0;
    bool button = false;
    while(connected_)
    {
      char buf[255];
      int len = read(fd, buf, sizeof(buf));
      if(0 < len)
      {
        for(int i = 0; i < len; i++)
        {
          if(buf[i] == 'e')
            button = true;
          else if(buf[i] == 'r' || buf[i] == 'o')
            button = false;
        }
      }

      int ret = -1;
      if(button)
      {
        if(!button_prev)
        {
          c = 0;
        }
        if(c % 10 == 0)
          ret = write(fd, "L0", 2); // LED off
        else if(c % 10 == 5)
          ret = write(fd, "L1", 2); // LED on
      }
      else
      {
        if(c % 10 == 0) ret = write(fd, "L1", 2);
      }
      button_prev = button;
      emergency_ = static_cast<bool>(button);

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      c++;
    }
    close(fd);
  });
}

} // namespace emergency_button
