import xbee
import sys
import time

TARGET_64BIT_ADDR = b'\x00\x13\xA2\x00\x42\x04\xA5\x68'

while True:
    received_msg = xbee.receive()
    if received_msg:
        emer = received_msg['payload'].decode()
        if emer == "E0":
            sys.stdout.write("r")
        elif emer == "E1":
            sys.stdout.write("e")
        elif emer == "N0":
            sys.stdout.write("o")
    
    recv_data = sys.stdin.read()
    if recv_data == "L0":
        try:
            xbee.transmit(TARGET_64BIT_ADDR, "L0")
        except Exception as e:
            pass
            # print("Transmit failure: %s" % str(e))
    
    elif recv_data == "L1":
        try:
            xbee.transmit(TARGET_64BIT_ADDR, "L1")
        except Exception as e:
            #print("Transmit failure: %s" % str(e))
            pass
