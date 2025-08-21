#include "blackjack.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    // Set console code page to UTF-8 so console knows how to interpret string data
    SetConsoleOutputCP(65001);
#endif
    Blackjack::Game game;
    game.play();
}