BUILDDIR=main/build

.PHONY: flash

all: help

help:
	@echo "available targets:"
	@echo "  build       :    compile firmware"
	@echo "  flash       :    flash firmware"
	@echo "  tty         :    connect to serial console"
	@echo "  clean       :    remove build artefacts"

clean:
	@rm -rf $(BUILDDIR)

build:
	@podman run \
		--rm \
		-v $$(pwd):/workspace \
		arduino-cli \
			arduino-cli compile -e \
			--fqbn esp8266:esp8266:d1_mini_clone \
			--libraries /workspace/src/TWAMP8266,/workspace/src/NTPClient \
			/workspace/main

flash:
	@podman run \
		--rm \
		-v $$(pwd):/workspace \
		--device=/dev/ttyUSB0 \
		arduino-cli \
			arduino-cli upload \
			-p /dev/ttyUSB0 \
			--fqbn esp8266:esp8266:d1_mini_clone \
			-i /workspace/main/build/esp8266.esp8266.d1_mini_clone/main.ino.bin

tty:
	@screen /dev/ttyUSB0 115200
