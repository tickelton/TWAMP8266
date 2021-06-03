BUILDDIR=main/build
MAIN_INO=main/main.ino
LIB_DIRS=libraries/NTPLight libraries/TWAMP8266
CPP_FILES=$(foreach libdir,$(LIB_DIRS),$(wildcard $(libdir)/*.cpp))
H_FILES= $(foreach libdir,$(LIB_DIRS),$(wildcard $(libdir)/*.h))

.PHONY: flash tty clean format-diff format

all: help

help:
	@echo "available targets:"
	@echo "  build       :    compile firmware"
	@echo "  flash       :    flash firmware"
	@echo "  tty         :    connect to serial console"
	@echo "  clean       :    remove build artefacts"
	@echo "  format-diff :    show a diff of the changes clang-format would make"
	@echo "  format      :    Inplace format source file with clang-format"

clean:
	@rm -rf $(BUILDDIR)

build:
	@podman run \
		--rm \
		-v $$(pwd):/workspace \
		arduino-cli \
			arduino-cli compile -e \
			--warnings all \
			--fqbn esp8266:esp8266:d1_mini_clone \
			--libraries /workspace/libraries \
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

format-diff:
	@clang-format \
		--assume-filename=main.cpp \
		--style=Google \
		$(MAIN_INO) \
		| diff -u $(MAIN_INO) - || true

	@$(foreach infile,$(CPP_FILES) $(H_FILES), clang-format \
		--style=Google \
		$(infile) \
		| diff -u $(infile) - || true\
		;)


format:
	@clang-format \
		-i \
		--assume-filename=main.cpp \
		--style=Google \
		$(MAIN_INO) \
		|| true

	@$(foreach infile,$(CPP_FILES) $(H_FILES), clang-format \
		-i \
		--style=Google \
		$(infile) \
		|| true\
		;)
