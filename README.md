# Cross-platform CLI Blackjack game made in C++17

Cross-platform (Linux, MacOS, Windows) Command line blackjack game built with C++17 using OOP principles as a way to improve modern C++ knowledge.

## Prerequisites
- For compilation and linking: GCC version 9 and up
- For build automation: Make
- For running unit tests: [CppUTest](https://cpputest.github.io/) (must be a system install)

## Installation
`git clone https://github.com/ZaRdEr15/cpp-blackjack.git`

## How to build
To build the application:
`make`

To build and run tests:
`make tests`

To build for release (has no debug information):
`make release`

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
