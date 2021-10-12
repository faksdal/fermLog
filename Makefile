PORT		= /dev/ttyUSB0
DEVICE		= esp8266:esp8266:d1_mini_pro
BUILD_PATH	= ./build
COMPILE		= arduino-cli compile  --warnings all
UPLOAD		= arduino-cli upload
SOURCE		= fermLog.ino

default: compile

compile:
	$(COMPILE) --fqbn $(DEVICE) --build-path $(BUILD_PATH) $(SOURCE)

upload:
	$(UPLOAD) --input-dir $(BUILD_PATH) --port $(PORT) --fqbn $(DEVICE)

