#pragma once
#include <algorithm>
#include <string>
#include <random>

namespace Blackjack {
    struct Card {
        uint8_t value {0};
        char face {'0'}; // number or |K|ing, |Q|ueen, |J|ack, |A|ce
        char suit {}; // ♣♦♥♠
        bool ace {false};
        Card(uint8_t v, char s);
        char valueToFace();
    };
    struct Player {
        std::vector<Card> hand;
    };
    class Game {
    private:
        std::random_device rd;
        std::default_random_engine rng;
    public:
        std::vector<Card> deck;
        void play();
        void shuffleCards();
        std::vector<Card> dealHand();
    };
}