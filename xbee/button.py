import xbee
import machine
import time
from machine import Pin

TARGET_NODE_ID = "EmerCoordinator"
TARGET_64BIT_ADDR = b'\x00\x13\xA2\x00\x42\x04\xA1\x84'

def find_device(node_id):
    try:
        for dev in xbee.discover():
            if dev['node_id'] == node_id:
                return dev
    
    except Exception as e:
        time.sleep(1)
    
    return None

button = Pin.board.D0 # Emergency button
#button.mode(Pin.IN)
#button.pull(Pin.PULL_UP)

becon = Pin.board.D5 # Green LED
#becon.mode(Pin.OUT)
becon.value(0) # Light off

power = Pin.board.D9 # Blue LED
#power.mode(Pin.OUT)
power.value(0) # Light off

power_status = 0
while True:
    if button.value() == 0:
        print("Please release emergency button")
    else:
        device = find_device(TARGET_NODE_ID)
        if not device:
            power.value(power_status)
            power_status = 1 - power_status
            
            print("Could not find the device with node identifier '%s'" % TARGET_NODE_ID)
        else:
            break

print("Found '%s'" % TARGET_NODE_ID)
power.value(1)

becon_count = 0
uncommunicate_count = 0
button_status = 0
button_status_prev = 0

while True:
    is_send = False
    is_communicated = False
    received_msg = xbee.receive()
    if received_msg:
        is_communicated = True
        led = received_msg['payload'].decode()
        if led == "L0":
            becon.value(0)
        elif led == "L1":
            becon.value(1)

    button_status = 1-button.value()
    if button_status != button_status_prev:
        status = "E%d"%(button_status)
        try:
            xbee.transmit(TARGET_64BIT_ADDR, status)
            is_send = True
        except Exception as e:
            print("Transmit failure: %s" % str(e))
    button_status_prev = button_status
    
    if (is_send == False) and (becon_count == 0):
        if button_status == True:
            status = "E1"
        else:
            status = "N0"
        try:
            xbee.transmit(TARGET_64BIT_ADDR, status)
        except Exception as e:
            print("Transmit failure: %s" % str(e))
    becon_count = becon_count + 1
    becon_count = becon_count % 2000
    
    if is_communicated == False:
        if uncommunicate_count > 5000:
            power.value(0)
        else:
            uncommunicate_count = uncommunicate_count + 1
    else:
        uncommunicate_count = 0
        power.value(1)
