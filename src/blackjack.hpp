#pragma once
#include <algorithm>
#include <random>

#define SUITS 4
#define SUIT_SIZE 13

namespace Blackjack {

    struct Card {
        int value;  // from 2 to 11
        std::string face;      // number or |K|ing, |Q|ueen, |J|ack, |A|ce
        std::string suit;      // ♣♦♥♠ (purely aesthetic)
        bool ace;       // false by default
        Card(const std::string f, const std::string s);
        void faceToValue(const std::string f);
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

    class Game {
        std::vector<Card> m_deck;

        /* Player m_player;
        Dealer m_dealer; */

        void fillDeck();
        void shuffleDeck(std::default_random_engine rng);
        void showDeck();
        void initialDeal();
        void decideWinner();
    public:
        void play();
    };
}
