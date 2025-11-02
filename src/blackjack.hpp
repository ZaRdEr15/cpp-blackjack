#pragma once
#include <algorithm>
#include <random>
#include <array>
#include <unordered_map>
#include <functional>

namespace Blackjack {

   inline constexpr int kBlackjack{ 21 };

   inline std::mt19937 g_mt{ std::random_device{}() };

   class Card {
    public:
      Card(std::string_view f, std::string_view s);

      std::string_view face;      // number or |K|ing, |Q|ueen, |J|ack, |A|ce
      std::string_view suit;      // ♣♦♥♠ (purely aesthetic)
      int value;  // from 1 to 10

    private:
      static const std::unordered_map<std::string_view, int> kFaceToValue;
   };

   class Deck {
    public:
      Deck();

      Card takeCard();
      bool needsShuffle() const;
      void shuffle();
      void showDeck() const; // debug

    private:
      static constexpr int kDeckSize{ 52 };
      static constexpr int kMinDeckSizeBeforeRefill{ kDeckSize / 3 };
      static constexpr std::array<std::string_view, 4> kSuit{ "♣", "♦", "♥", "♠" };
      static constexpr std::array<std::string_view, 13> kSingleSuitFaces{
         "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
      };

      std::vector<Card> m_cards;
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

      std::vector<Card> m_hand;
      bool m_finished;

    private:
      bool hasAce() const;
      void calculateTotalValue();

      int m_total_value;
   };

   class Player : public HandHolder {
    public:
      Player();
      
      void processAction(char action, Deck& deck);
      std::string getCardsString() const;

      //std::vector<std::vector<Card>> additional_hands;

    private:
      void doubleDown(Deck& deck);  // increase bet by 100% and take exactly one card, then stand
      void split(); // split cards into two separate hands

      std::unordered_map<char, std::function<void(Deck&)>> m_action_map;
   };

   // stands on 17 and higher (no matter what)
   class Dealer : public HandHolder {
    public:
      Dealer();

      void playHand(int player_total, Deck& deck);
      std::string getCardsString(bool player_turn_finished) const;

    private:
      static constexpr int kDealerStand{ 17 };
   };

   class Game {
    public:
      Game();

      void play();

    private:
      static constexpr std::string_view kPossibleActions{ "hsdp" };
   
      void clearTerminal() const;
      void displayGameState() const;
      bool isValidInput();
      bool isValidAction(char action) const;
      void handlePlayerInput();
      void playerTurn();
      void dealerTurn();
      void decideWinner() const;

      Deck m_deck;
      Dealer m_dealer;
      Player m_player;
   };
}
