BOARD_TAG = uno
ARDUINO_PORT = /dev/ttyACM0

ARDUINO_LIBS = Wire Wire/utility Time/Time Time/DS1307RTC LiquidCrystal_I2C
MAKEFILE_DIR = ../Arduino-Makefile

ARDUINO_DIR = /usr/share/arduino
ARDMK_DIR = $(MAKEFILE_DIR)
AVR_TOOLS_DIR = /usr

include $(MAKEFILE_DIR)/arduino-mk/Arduino.mk
