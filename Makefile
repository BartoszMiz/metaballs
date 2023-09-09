CC = gcc
CFLAGS = -Wall -Wpedantic -g
LIBS = -lraylib -lGL -lm
BUILD_DIR = build
TARGET = metaballs

SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

.PHONY: all
all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: ./%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ_FILES)

.PHONY: run
run: all
	./$(TARGET)