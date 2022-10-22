#include <emergency_button/WiredEmergencyButton.h>
#include <iostream>
#include <thread>

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

#define USBIO_DEFAULT_BUTTON_ID 6

namespace emergency_button
{

void usbio_button(usbio_t * hd, int dev_num, int button_id) {}

WiredEmergencyButton::WiredEmergencyButton() {}

WiredEmergencyButton::~WiredEmergencyButton()
{
  running_ = false;
  if(th_.joinable())
  {
    th_.join();
  }
}

void WiredEmergencyButton::connect()
{
  auto button_id = USBIO_DEFAULT_BUTTON_ID;
  th_ = std::thread([button_id, this]() {
    std::cout << "Looking for non-wireless button..." << std::endl;
    int dev_num;
    usbio_t hd[MAX_USBIO2_NUM];
    bool ret = usbio_init(hd, &dev_num, false);

    if(!ret || (dev_num == 0))
    {
      throw std::runtime_error("failed to open wired/wireless emergency button");
    }
    std::cout << "found wired emergency button (number of button = " << dev_num << ")" << std::endl;

    int usb_id = -1;
    int is_on[2];
    int led_on = 0;
    int led_off = 1;
    uint8_t recv;
    for(int i = 0; i < dev_num; i++)
    {
      usbio_read(hd[i], &recv);
      int id = (recv & 0x0e) >> 1;
      if(id == button_id)
      {
        usb_id = i;
        is_on[0] = 1;
        is_on[1] = 0;
        led_on = 0;
        led_off = 1;
        std::cout << "detect user customized button id=" << id << std::endl;
      }
    }

    bool button_prev = false;
    int c = 0;

    while(running_)
    {
      uint8_t recv;
      bool button = false;
      if(!usbio_read(hd[usb_id], &recv))
      {
        std::cerr << "Could not read button";
        button = true;
      }
      else
      {
        button = is_on[recv & 0x01];
      }
      if(button)
      {
        if(!button_prev)
        {
          c = 0;
        }
        usbio_write(hd[usb_id], (c % 10 <= 5) ? led_off : led_on, 0x01); // button is pushed
      }
      else
      {
        usbio_write(hd[usb_id], led_on, 0x01); // button is not pushed, function is ready
      }
      button_prev = button;
      emergency_ = static_cast<bool>(button);

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      c++;
    }
    usbio_cleanup(hd[usb_id], true);
  });
}

} // namespace emergency_button
