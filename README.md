# Cross-platform CLI Blackjack game made in C++17

Cross-platform (Linux, MacOS, Windows) Command line blackjack game built with C++17 using OOP principles as a way to improve modern C++ knowledge.

## Prerequisites
- For compilation and linking: GCC version 9 and up
- For build automation: CMake and Ninja
- For running unit tests: [CppUTest](https://cpputest.github.io/) (fetched using CMake)

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

To run tests (inside build_debug/tests or build_release/tests):
`ctest`

## Usage

Running the application choose any of the actions inside the parentheses (single character) and hit enter.

To stop the application, press CTRL+C to kill the process.

## Roadmap
- [ ] Make it possible to unit test functions but not break OOP principles
- [ ] Show dealer single card value before reveal
- [ ] Add betting
- [ ] Add split action
- [ ] Add insurance (only if up card is an ace)

## License
Distributed under MIT License. See LICENSE for more information. 
