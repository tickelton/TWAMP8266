TWAMP8266
=========


Building the container
----------------------

podman build -t arduino-cli .


Compiling
---------

podman run \
    --rm \
    -v $(pwd):/workspace \
    arduino-cli \
        arduino-cli compile -e \
        --fqbn esp8266:esp8266:d1_mini_clone \
        --libraries /workspace/src/twamp8266 \
        /workspace/main

Output:
Executable segment sizes:
ICACHE : 32768           - flash instruction cache
IROM   : 231772          - code in flash         (default or ICACHE_FLASH_ATTR)
IRAM   : 26777   / 32768 - code in IRAM          (IRAM_ATTR, ISRs...)
DATA   : 1496  )         - initialized variables (global, static) in RAM/HEAP
RODATA : 876   ) / 81920 - constants             (global, static) in RAM/HEAP
BSS    : 25608 )         - zeroed variables      (global, static) in RAM/HEAP
Sketch uses 260921 bytes (24%) of program storage space. Maximum is 1044464 bytes.
Global variables use 27980 bytes (34%) of dynamic memory, leaving 53940 bytes for local variables. Maximum is 81920 bytes.


Flashing
--------

docker run \
    --rm \
    -v $(pwd):/workspace \
    --device=/dev/ttyUSB0 \
    arduino-cli \
        arduino-cli upload \
        -p /dev/ttyUSB0 \
        --fqbn esp8266:esp8266:d1_mini_clone \
        -i /workspace//main/build/esp8266.esp8266.d1_mini_clone/main.ino.bin

Output:
esptool.py v3.0
Serial port /dev/ttyUSB0
Connecting....
Chip is ESP8266EX
Features: WiFi
Crystal is 26MHz
MAC: d8:f1:5b:10:ce:8e
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Auto-detected Flash size: 4MB
Compressed 265072 bytes to 195076...
Writing at 0x00000000... (8 %)
[...]
Writing at 0x0002c000... (100 %)
Wrote 265072 bytes (195076 compressed) at 0x00000000 in 4.4 seconds (effective 478.5 kbit/s)...
Hash of data verified.
Leaving...
Hard resetting via RTS pin...

