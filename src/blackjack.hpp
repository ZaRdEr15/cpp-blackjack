#pragma once
#include <algorithm>
#include <random>
#include <array>

namespace Blackjack {
    class Game {
        enum class Suit {Spades, Hearts, Clubs, Diamonds};

        struct Card {
            uint8_t value;  // from 2 to 11
            char face;      // number or |K|ing, |Q|ueen, |J|ack, |A|ce
            char suit;      // ♣♦♥♠ (purely aesthetic)
            bool ace;       // false by default
            Card(uint8_t v, Suit s);
            void faceToValue(char f);
        };

        class HandHolder {
        public:
            std::vector<Card> hand;
            bool finished;
            void hit();
            void stand();
            virtual void calculateTotalValue();
            virtual void showCards();
        };

        class Player : HandHolder {
        public:
            void showCards() override; // shows all cards
            void doubleDown(); // increase bet by 100% and take exactly one card, then stand
            void split();
            void chooseAction();
        };

        class Dealer : HandHolder { // stands on 17 and higher (no matter what)
        public:
            void calculateTotalValue() override;
            void showCards() override; // shows 1 card before player finished and all after finished
        };

        std::random_device m_rd;
        std::default_random_engine m_rng;

        std::array<Card, 52> m_default_deck;

        Player player;
        Dealer dealer;

        void fillDeck();
        void shuffleCards();
        void initialDeal();
        void decideWinner();
    public:
        static void play();
    };
}