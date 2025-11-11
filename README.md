# Cross-platform CLI Blackjack game made in C++17

Cross-platform (Linux, MacOS, Windows) Command line blackjack game built with C++17 using OOP principles as a way to improve modern C++ knowledge.

## Prerequisites

- For compilation and linking: GCC version 9 and up
- For build automation: [CMake](https://cmake.org/) and [Ninja](https://github.com/ninja-build/ninja)
- [CppUTest](https://cpputest.github.io/)

## Installation

`git clone https://github.com/ZaRdEr15/cpp-blackjack.git`

## How to build

To build the application (builds tests also):
```
mkdir build_debug
cd build_debug
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja
```

To build for release (has no debug information):
```
mkdir build_release
cd build_release
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja
```

To run tests:
```
cd build_debug
ctest
```

## Usage

Running the application choose any of the actions inside the parentheses (single character) and hit enter.

To stop the application, press CTRL+C to kill the process.

## Blackjack rules



## Roadmap
- [ ] Add Blackjack simplified rules
- [ ] Split into modular parts for easy readability
- [ ] Make it possible to unit test functions but not break OOP principles
- [ ] Show dealer single card value before reveal
- [ ] Add betting
- [ ] Add split action
- [ ] Add insurance (only if up card is an ace)

## License
Distributed under MIT License. See LICENSE for more information. 
