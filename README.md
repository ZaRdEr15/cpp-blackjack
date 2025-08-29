# CLI Blackjack game made in C++17

Command line blackjack game built with C++17 using OOP principles as a way to improve modern C++ knowledge.

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
- [ ] Make string to be outputted in the showCards be constructed beforehand using stringstream
- [ ] Apply SOLID principles (Game as outer class, Card, HandHolder, Player, Dealer as inner classes)
- [ ] Make it possible to unit test functions but not break OOP principles
- [ ] Add betting
- [ ] Improve visual to make tabular in one row, Player hand and Dealer hand visible while action is at the bottom
- [ ] Add split action
- [ ] Add insurance (only if up card is an ace)

## License
Distributed under MIT License. See LICENSE for more information. 
