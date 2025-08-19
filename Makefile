CXX          := g++
CXXFLAGS     := -pedantic-errors -Wall -Wextra -Werror -std=c++17
BUILD        := ./build
INCLUDE      := -Isrc
SRC          := $(wildcard src/*.cpp)
OBJECTS      := $(SRC:src/%.cpp=$(BUILD)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)
TARGET       := blackjack

all: build $(TARGET)

# $@ - target name
# $< first prerequisite
# $^ all prerequisites
$(BUILD)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^

-include $(DEPENDENCIES)

.PHONY: all build clean info

build:
	@if [ ! -d "build" ]; then mkdir -p build; fi

clean:
	-@rm -rvf $(BUILD)/*

info:
	@echo $(SRC)
	@echo $(OBJECTS)
	@echo $(DEPENDENCIES)