#pragma once
#include <algorithm>
#include <random>
#include <array>

namespace Blackjack {

    inline constexpr int Blackjack     {21};
    inline constexpr int DealerStand   {17};
    inline constexpr int RefillDeck    {52 / 3};

    inline constexpr std::array<std::string_view, 4> Suit {"♣", "♦", "♥", "♠"};
    inline constexpr std::array<std::string_view, 13> SingleSuitFaces {
        "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
    };

    struct Card {
        int value;  // from 1 to 10
        std::string_view face;      // number or |K|ing, |Q|ueen, |J|ack, |A|ce
        std::string_view suit;      // ♣♦♥♠ (purely aesthetic)

        Card(std::string_view f, std::string_view s);
        void faceToValue(std::string_view f);
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
        void playHand(const int& player_total);
        void showCards() override; // shows 1 card before player finished and all after finished
    };

    class Game {
        std::random_device rd;
        std::default_random_engine rng;
        std::vector<Card> initialDeal();
        void fillDeck();
        void showDeck();
        void decideWinner(const Player& player, const Dealer& dealer);
    public:
        Game();
        void play();
        static Card takeCard(); // take a card from the deck, lowering deck count
    };
}
