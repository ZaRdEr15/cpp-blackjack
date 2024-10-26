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
        int total_value;
        bool finished;

        HandHolder(Card first_card, Card second_card);
        void hit();
        void stand();
        virtual void calculateTotalValue();
        virtual void showCards();
    };

    class Player : HandHolder {
    public:
        //std::vector<std::vector<Card>> additional_hands;

        Player(Card first_card, Card second_card);
        //void doubleDown(); // increase bet by 100% and take exactly one card, then stand
        //void split(); // split cards into two separate hands
        void showCards() override;
        void chooseAction();
    };

    class Dealer : HandHolder { // stands on 17 and higher (no matter what)
    public:
        void calculateTotalValue() override;
        void showCards() override; // shows 1 card before player finished and all after finished
    };

    class Game {
        void initialDeal();
        void decideWinner();
    public:
        void play();
        void fillDeck();
        void shuffleDeck(std::default_random_engine rng);
        void showDeck();
        Card takeCard();
    };
}
