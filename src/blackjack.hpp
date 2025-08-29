#pragma once
#include <algorithm>
#include <random>
#include <array>
#include <unordered_map>
#include <functional>

namespace Blackjack {

    inline constexpr int Blackjack {21};
    inline constexpr int DealerStand {17};
    inline constexpr int MinDeckSizeBeforeRefill {52 / 3};

    inline constexpr std::array<std::string_view, 4> Suit {"♣", "♦", "♥", "♠"};
    inline constexpr std::array<std::string_view, 13> SingleSuitFaces {
        "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
    };

    inline std::mt19937 mt {std::random_device {}()};

    inline constexpr std::string_view PossibleActions {"hsdp"};

    // forward declarations
    class Card;
    class Deck;
    class HandHolder;
    class Player;
    class Dealer;
    class Game;

    class Card {
    public:
        int value;  // from 1 to 10
        std::string_view face;      // number or |K|ing, |Q|ueen, |J|ack, |A|ce
        std::string_view suit;      // ♣♦♥♠ (purely aesthetic)

        Card(std::string_view f, std::string_view s);
    private:
        void faceToValue(std::string_view f);
    };

    class Deck {
    public:
        std::vector<Card> deck;

        Deck();
        std::vector<Card> initialDeal();
        Card takeCard(); // take a card from the deck, lowering deck count
        void fillAndShuffleDeck();
        void showDeck();
    private:
    };

    // abstract base class, objects are not allowed
    class HandHolder {
    public:
        int total_value;
        bool finished;

        HandHolder(std::vector<Card> initial_hand);
        virtual ~HandHolder(); // always include virtual destructor of a base polymorphic class
        virtual void showCards() = 0; // pure virtual function
    protected:
        std::vector<Card> hand;

        virtual std::string getCardsString() = 0;
        void hit(Deck& deck_instance);
        void stand();
    private:
        bool hasAce();
        void calculateTotalValue();
    };

    class Player : public HandHolder {
    public:
        //std::vector<std::vector<Card>> additional_hands;

        Player(std::vector<Card> initial_hand);
        void showCards() override;
        void processAction(const char& action, Deck& deck_instance);
    private:
        std::unordered_map<char, std::function<void(Deck&)>> action_map;

        std::string getCardsString() override;
        void doubleDown(Deck& deck_instance);  // increase bet by 100% and take exactly one card, then stand
        void split();       // split cards into two separate hands
    };

    class Dealer : public HandHolder { // stands on 17 and higher (no matter what)
    public:
        Dealer(std::vector<Card> initial_hand);
        void playHand(const int& player_total, Deck& deck_instance);
        void showCards() override; // shows 1 card before player finished and all after finished
    private:
        bool show_once; // show one card hidden even if dealer has 21 or after game won/lost first round

        std::string getCardsString() override;
    };

    class Game {
    public:
        friend HandHolder; // Gives HandHolder access to takeCard()

        Game();
        void play();
    private:
        Deck deck;

        void displayGameState(Dealer& dealer_instance, Player& player_instance);
        bool isValidInput();
        bool isValidAction(const char& action);
        void handlePlayerInput(Player& player_instance);
        void playerTurn(Dealer& dealer_instance, Player& player_instance);
        void dealerTurn(Dealer& dealer_instance, Player& player_instance);
        void decideWinner(const Player& player, const Dealer& dealer);
    };
}
