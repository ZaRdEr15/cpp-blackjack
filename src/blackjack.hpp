#pragma once
#include <algorithm>
#include <random>

namespace Blackjack {

    struct Card {
        int value;  // from 2 to 11
        std::string face;      // number or |K|ing, |Q|ueen, |J|ack, |A|ce
        std::string suit;      // ♣♦♥♠ (purely aesthetic)

        Card(const std::string f, const std::string s);
        void faceToValue(const std::string f);
    };

    class HandHolder {
    public:
        std::vector<Card> hand;
        int total_value;
        bool finished;

        HandHolder(std::vector<Card> initial_hand);
        void hit();
        void stand();
        bool hasAce();
        void calculateTotalValue();
        virtual void showCards();
    };

    class Player : public HandHolder {
    public:
        //std::vector<std::vector<Card>> additional_hands;

        Player(std::vector<Card> initial_hand);
        void doubleDown();  // increase bet by 100% and take exactly one card, then stand
        void split();       // split cards into two separate hands
        void showCards() override;
        void chooseAction();
    };

    class Dealer : public HandHolder { // stands on 17 and higher (no matter what)
    public:
        Dealer(std::vector<Card> initial_hand);
        void playHand();
        void showCards() override; // shows 1 card before player finished and all after finished
    };

    class Game {
        std::vector<Card> initialDeal();
        void fillDeck();
        void shuffleDeck(std::default_random_engine rng);
        void showDeck();
        void decideWinner(const Player& player, const Dealer& dealer);
    public:
        void play();
        static Card takeCard(); // take a card from the deck, lowering deck count
    };
}
