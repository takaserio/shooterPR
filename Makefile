TARGET_EXEC := shooter

SRC_DIR := ./src
BUILD_DIR := ./build

SRCS := $(shell find $(SRC_DIR) -name '*cpp')

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(TARGET_EXEC): $(SRCS)
	g++ -o $@ $^