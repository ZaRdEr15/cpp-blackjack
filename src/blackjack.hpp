#pragma once
#include <algorithm>
#include <random>
#include <array>
#include <unordered_map>
#include <functional>

namespace Blackjack {

   inline constexpr int Blackjack{ 21 };

   inline std::mt19937 mt{ std::random_device{}() };

   class Card {
    public:
      Card(std::string_view f, std::string_view s);

      std::string_view face;      // number or |K|ing, |Q|ueen, |J|ack, |A|ce
      std::string_view suit;      // ♣♦♥♠ (purely aesthetic)
      int value;  // from 1 to 10

    private:
      static const std::unordered_map<std::string_view, int> FaceToValue;
   };

   class Deck {
    public:
      Deck();

      Card takeCard();
      bool needsShuffle() const;
      void shuffle();
      void showDeck() const; // debug

    private:
      static constexpr int DeckSize{ 52 };
      static constexpr int MinDeckSizeBeforeRefill{ DeckSize / 3 };
      static constexpr std::array<std::string_view, 4> Suit{ "♣", "♦", "♥", "♠" };
      static constexpr std::array<std::string_view, 13> SingleSuitFaces{
         "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
      };

      std::vector<Card> cards;
   };

   // abstract base class, objects are not allowed
   class HandHolder {
    public:
      HandHolder();
      virtual ~HandHolder(); // always include virtual destructor of a base polymorphic class

      void startNewHand(Deck& deck);
      int getTotalValue() const;
      bool isFinished() const;

    protected:
      void hit(Deck& deck);
      void stand();

      std::vector<Card> hand;
      bool finished;

    private:
      bool hasAce();
      void calculateTotalValue();

      int total_value;
   };

   class Player : public HandHolder {
    public:
      Player();
      
      void processAction(char action, Deck& deck);
      std::string getCardsString();

      //std::vector<std::vector<Card>> additional_hands;

    private:
      void doubleDown(Deck& deck);  // increase bet by 100% and take exactly one card, then stand
      void split(); // split cards into two separate hands

      std::unordered_map<char, std::function<void(Deck&)>> action_map;
   };

   // stands on 17 and higher (no matter what)
   class Dealer : public HandHolder {
    public:
      Dealer();

      void playHand(int player_total, Deck& deck);
      std::string getCardsString(bool player_turn_finished);

    private:
      static constexpr int DealerStand {17};
   };

   class Game {
    public:
      Game();

      void play();

    private:
      static constexpr std::string_view PossibleActions{ "hsdp" };
   
      void clearTerminal();
      void displayGameState();
      bool isValidInput();
      bool isValidAction(char action);
      void handlePlayerInput();
      void playerTurn();
      void dealerTurn();
      void decideWinner();

      Deck deck;
      Dealer dealer;
      Player player;
   };
}
