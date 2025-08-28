CXX          := g++
CXXFLAGS     := -pedantic-errors -Wall -Wextra -Werror -std=c++17
LDLIBS       := -lCppUTest

BUILD_DIR    := build
SRC_DIR      := src
TEST_DIR     := tests

CXXFLAGS += -I$(SRC_DIR)

APP_SRC      := $(SRC_DIR)/blackjack.cpp
APP_OBJECTS  := $(APP_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
APP_TARGET   := blackjack

TEST_SRC     := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS := $(TEST_SRC:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_TARGET  := blackjack_tests

DEPENDENCIES := $(APP_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

.PHONY: all build tests release clean info

all: CXXFLAGS += -g -O0
all: build $(APP_TARGET)

tests: CXXFLAGS += -g -O0
tests: build $(TEST_TARGET)
	./$(TEST_TARGET) -c

# $@ - target name
# $< first prerequisite
# $^ all prerequisites
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -MMD -o $@

$(APP_TARGET): $(APP_OBJECTS) $(BUILD_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -MMD -o $@

$(TEST_TARGET): $(TEST_OBJECTS) $(APP_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

-include $(DEPENDENCIES)

build:
	@if [ ! -d "build" ]; then mkdir -p build; fi

release: CXXFLAGS += -O3
release: build $(APP_TARGET)

clean:
	-@rm -rvf $(BUILD_DIR) $(APP_TARGET) $(TEST_TARGET)

info:
	@echo $(APP_SRC)
	@echo $(APP_OBJECTS)
	@echo $(TEST_SRC)
	@echo $(TEST_OBJECTS)
	@echo $(DEPENDENCIES)