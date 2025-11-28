# Cross-platform CLI Blackjack game made in C++17

Cross-platform (Linux, MacOS, Windows) Command line blackjack game built with C++17 using OOP principles as a way to improve modern C++ knowledge.

## Prerequisites

- For C++17 compilation and linking: GCC version 9 and up
- For build automation: [CMake](https://cmake.org/) and [Ninja](https://github.com/ninja-build/ninja)
- For unit testing: [CppUTest](https://cpputest.github.io/)

## Installation

`git clone https://github.com/ZaRdEr15/cpp-blackjack.git`

## How to build

To build the application (builds tests also):
```
cmake -B build_debug -G Ninja --preset debug
cmake --build build_debug
```

To build for release:
```
cmake -B build_release -G Ninja --preset release
cmake --build build_release
```

To run tests:
```
ctest --test-dir build_debug
```

## Usage

Run **blackjack** application from either *build_debug* or *build_release*.

Choose any of the actions inside the parentheses (single character) and hit enter.

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
- [ ] Add a graphical interface using raylib

## License
Distributed under MIT License. See LICENSE for more information. 
