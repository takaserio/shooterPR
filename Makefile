TARGET_EXEC := shooter

SRC_DIR := ./src
BUILD_DIR := ./build

SRCS := $(shell find $(SRC_DIR) -name '*c')

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

$(TARGET_EXEC): ./src/main.c
	gcc -o $@ -g $^

t:	
	@echo $(SRCS)