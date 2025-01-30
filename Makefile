CC = gcc
SRC = src/vdrop.c
BUILD_DIR = build
OBJF = $(BUILD_DIR)/vdrop.o
TARGET = $(BUILD_DIR)/vdrop

.PHONY: default clean

default: $(TARGET)

$(BUILD_DIR):
	@rm -rf $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)

$(OBJF): $(SRC) | $(BUILD_DIR)
	$(CC) -c $(SRC) -o $(OBJF)

$(TARGET): $(OBJF)
	$(CC) $(OBJF) -o $(TARGET)

clean:
	@rm -rf $(BUILD_DIR)