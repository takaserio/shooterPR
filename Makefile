TARGET_EXEC := shooter

SRC_DIR := ./src
BUILD_DIR := ./build

SRCS := $(shell find $(SRC_DIR) -name '*cpp')

OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	ld $^ -o $@

$(BUILD_DIR)/Character.o: $(SRC_DIR)/Character.cpp
	g++ -o $@ -c $^

$(BUILD_DIR)/Enemy.o: $(SRC_DIR)/Enemy.cpp
	g++ -o $@ -c $^

$(BUILD_DIR)/EnemyManager.o: $(SRC_DIR)/EnemyManager.cpp
	g++ -o $@ -c $^

$(BUILD_DIR)/Layzer.o: $(SRC_DIR)/Layzer.cpp
	g++ -o $@ -c $^

$(BUILD_DIR)/LayzerManager.o: $(SRC_DIR)/LayzerManager.cpp
	g++ -o $@ -c $^

$(BUILD_DIR)/NormalEnemy.o: $(SRC_DIR)/NormalEnemy.cpp
	g++ -o $@ -c $^

$(BUILD_DIR)/Player.o: $(SRC_DIR)/Player.cpp
	g++ -o $@ -c $^

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	g++ -o $@ -c $^

