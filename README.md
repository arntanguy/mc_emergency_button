This package publishes the status of the USB emergency button as a ROS topic, and controls the behaviour of its blinking LED.

Supported:
- Green "soft emergency" button
# mc_emergency_button

This package contains all the tools needed to communicate with emergency buttons:
- A library to get the status of buttons over serial ports
- A tool to check the status of buttons
- An `mc_rtc` plugin to make these buttons available to the controller

## Supported Buttons

- Green "soft emergency" button
- Wireless button: https://github.com/isri-aist/wiki/blob/master/hrg-exp-environment/wireless-emer-button.md

## Installation

```sh
mkdir build
cd build
cmake ..
make
sudo make install
```

### Buttons setup

To be allowed access to the buttons as a regular user, you need:
- To be in the dialout group
- The following udev rules in `cat /etc/udev/rules.d/51-usb-serial.rules
```
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015", GROUP="dialout", MODE="0660", SYMLINK+="wireless_emergency_button"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1352", ATTRS{idProduct}=="0121", GROUP="dialout", MODE="0660", SYMLINK+="wired_emergency_button"
```