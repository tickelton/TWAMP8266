# TWAMP8266

An Arduino based ESP8266 TWAMP Light reflector.

## Overview

**TWAMP8266** is an ESP8266 firmware acting as a TWAMP Light reflector that can be used to measure WiFi performance using TWAMP capable monitoring and diagnostics solutions like for example [check_twamp-light](https://github.com/tickelton/monitoring-plugins/tree/master/check_twamp-light).

TWAMP8266 is developed for and tested on [LOLIN/Wemos D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) and [NodeMCU](https://www.nodemcu.com/index_en.html) but should work on all ESP8266 boards that are supported by [Arduino](https://www.nodemcu.com/index_en.html).

In addition to the source code for the actual firmware, this repository also contains a Containerfile for setting up the required [arduino-cli](https://arduino.github.io/arduino-cli/) development environment inside a [Podman](https://podman.io/) or [Docker](https://www.docker.com/) container.

### NTP server and TWAMP port

To create accurate timestamps for the TWAMP packets, TWAMP8266 includes an NTP client and by default gets its time from **ntp.ubuntu.com**. If you want to use a different time server, it can be changed in *libraries/NTPLight/NTPLight.h*.

By default TWAMP8266 listens on port *862*. If necessary the port can be changed in *libraries/TWAMP8266/TWAMP8266.h*.

## Building the arduino-cli container

### A note on Podman and Docker

The container images used in this repository are designed for and typically used with Podman.  
Since Podman Containerfiles and Docker Dockerfile are largely compatible, the following instructions are equally applicable to Docker. The only change required to use Docker instead of Podman is that it is necessary to either rename *Containerfile* to *Dockerfile* or to explicitly specify the input file with the *-f* parameter, e.g. `docker build -f Containerfile .` instead of `podman build .`.

### Building the container

An arduino-cli container can be built and tagged as follows:
```
podman build -t arduino-cli .
```

## Compiling the firmware

When using **Podman** and the **arduino-cli** container the firmware can be built by simply running
```
make
```

The resulting firmware binary can be found at *main/build/esp8266.esp8266.d1_mini_clone/main.ino.bin*.

If a native **arduino-cli** installation is to be used or different build options are required, the appropriate build command can be run manually in a fashion similar to
```
podman run \
    --rm \
    -v $(pwd):/workspace \
    arduino-cli \
        arduino-cli compile -e \
        --fqbn esp8266:esp8266:d1_mini_clone \
        --libraries /workspace/libraries \
        /workspace/main
```

## Flashing the firmware

The firmware can be flashed using the command
```
make flash
```

This assumes that the target device is connected to */dev/ttyUSB1*.  
See **Makefile** for the exact **arduino-cli** command used.


## Connecting to the serial console

Once the firmware is running on the ESP8266, a connection to the serial console can be established with screen:
```
screen /dev/ttyUSB0 115200
```

There is also a convenient make target that does exactly the same thing:
```
make tty
```

## Configuring the WiFi credentials

On first boot the ESP8266 will not be able to connect to your WiFi since the network name and password are not set by default.

To set the WiFi credentials connect to the serial console and interrupt normal startup by hitting *Return* within the first 5 seconds after device startup. You will be presented with the following prompt that allows you to configure your network.

```

Press Return to enter WiFi configuration.
Enter number :
 [1] Change network name [Fritz!Box Fon Wlan 1970].
 [2] Change Password.
 [3] Exit and continue startup.
> 
```

## License

TWAMP8266 is licensed under the terms of the MIT license.  
See **LICENSE** for the full license text.
