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
```sh
sudo useradd $USER dialout
```
You need to logout and log back in for this change to take effect.
- The following udev rules in `cat /etc/udev/rules.d/51-usb-serial.rules
```
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6015", GROUP="dialout", MODE="0660", SYMLINK+="wireless_emergency_button"
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1352", ATTRS{idProduct}=="0121", GROUP="dialout", MODE="0660", SYMLINK+="wired_emergency_button"
```
You can reload the udev rules with
```sh
sudo udevadm control --reload-rules
sudo udevadm trigger
```

## Usage

The plugin uses shared memory to get the status of the emergency button.

First, start the server:

```sh
EmergencyButtonServer ~/src/mc_emergency_button/plugin/etc/EmergencyButtonPlugin.yaml
```

This will read the button chosen in the configuration file, and write its state to shared memory.

Then start a controller using the `EmergencyButtonPlugin`

```
Plugins: [EmergencyButtonPlugin]
```

You can now read the button status in the datastore as

```cpp
// true if emergency is pressed
auto buttonStatus = ctl.datastore().call<bool>("EmergencyButton::State"); 
```

### A note on configuration

The configuration file looks like:

```yaml
# When required = true, the button must be connected at all times. If the button is disconnected, it will be considered as an emergency
# When false, it is only an emergency when the button is connected and is pressed
required: false
type: "wireless" # supported types: "wireless" or "wired"
port: "/dev/wireless_emergency_button" # serial port corresponding to the button
timeout: 1000 # Time in ms after which the button is considered disconnected if no new status has been received
gui: true
```
