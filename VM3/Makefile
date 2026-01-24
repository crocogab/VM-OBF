CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc/vm
LDFLAGS =

SRC_DIR = src
VM_DIR = $(SRC_DIR)/vm
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.c $(VM_DIR)/vm.c
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/vm.o

TARGET = $(BUILD_DIR)/vm

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(VM_DIR)/vm.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/vm.o: $(VM_DIR)/vm.c $(VM_DIR)/vm.h $(VM_DIR)/opcodes.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run
