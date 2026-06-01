CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -g -pthread -D_POSIX_C_SOURCE=200809L
INCLUDE := -Iinclude

TARGET  := os_sim
BUILD   := build

# Collect every .c file under src/ recursively
SRCS := $(shell find src -name '*.c')
OBJS := $(patsubst src/%.c, $(BUILD)/%.o, $(SRCS))

.PHONY: all run clean debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Build successful -> ./$(TARGET)"

# Compile each source, mirroring the src/ subdirectory tree under build/
$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

run: all
	./$(TARGET)

debug: CFLAGS += -DDEBUG -fsanitize=address,undefined
debug: all

clean:
	rm -rf $(BUILD) $(TARGET)
	@echo "Cleaned."
