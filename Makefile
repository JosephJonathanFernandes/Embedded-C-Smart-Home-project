CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
TARGET = smart_home_sim

all: $(TARGET)

$(TARGET): smart_home_sim.c
	$(CC) $(CFLAGS) -o $(TARGET) smart_home_sim.c

clean:
	rm -f $(TARGET) *.o eeprom.bin

.PHONY: all clean
