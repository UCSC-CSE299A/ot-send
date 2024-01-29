| Supported Targets | ESP32-C6 | ESP32-H2 |
| ----------------- | -------- | -------- |

# OpenThread UDP Sender

This codebase enables an 802.15.4 ESP32 MCU to broadcast UDP packets in a Thread WLAN

## Setup

You will first need to form a Thread WLAN with at least two devices.

On the sending device, wait until it has successfully connected to the Thread WLAN. If the sending device is working properly, you should expect the following output, which will keep printing in an infinite loop:
```
I(49027) OPENTHREAD:[N] Platform------: ************************************
I(49027) OPENTHREAD:[N] Platform------: UDP packet successfully sent.
I(49027) OPENTHREAD:[N] Platform------: ************************************
```

All other devices in the networks will act as the receivers. Set up the UDP socket on each receiving device to listen to port `12345`.
```bash
udp open
udp bind :: 1235
```

You should then to expect to see the following output from each of the devices:
```
22 bytes from fdc2:53d3:bb7e:2437:a847:67e6:f17e:f186 12345 Packet Number 1
22 bytes from fdc2:53d3:bb7e:2437:a847:67e6:f17e:f186 12345 Packet Number 2
22 bytes from fdc2:53d3:bb7e:2437:a847:67e6:f17e:f186 12345 Packet Number 3
22 bytes from fdc2:53d3:bb7e:2437:a847:67e6:f17e:f186 12345 Packet Number 4
...
```

Note that the `Packet Number` may vary depending on when you have created the UDP socket.
