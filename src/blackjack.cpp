#include <iostream>
#include <limits>   // std::numeric_limits
#include <sstream>  // std::stringstream
#include <thread>   // std::this_thread::sleep_for
#include <chrono>   // std::chrono::seconds
#include "blackjack.hpp"

namespace Blackjack {

    Card::Card(std::string_view f, std::string_view s) : face{ f }, 
                                                         suit{ s }, 
                                                         value{ FaceToValue.at(f) } {}

    const std::unordered_map<std::string_view, int> Card::FaceToValue{
        {"2",  2}, {"3",  3}, {"4", 4}, {"5",   5}, {"6",  6},
        {"7",  7}, {"8",  8}, {"9", 9}, {"10", 10}, {"J", 10}, 
        {"Q", 10}, {"K", 10}, {"A", 1}
    };

    Deck::Deck() {
        shuffle();
    }

    Card Deck::takeCard() {
        Card card{ cards.back() };
        cards.pop_back();
        return card;
    }

    bool Deck::needsShuffle() const {
        return cards.size() <= MinDeckSizeBeforeRefill;
    }

    void Deck::shuffle() {
        cards.clear();
        for (auto suit : Suit) {
            for (auto face : SingleSuitFaces) {
                cards.push_back(Card{ face, suit });
            }
        }
        std::shuffle(cards.begin(), cards.end(), mt);
    }

    void Deck::showDeck() const {
        std::cout << "Deck (" << cards.size() << "):" << '\n';
        for (const auto& card : cards) {
            std::cout << card.face << card.suit << ' ';
        }
    }

    HandHolder::HandHolder() : hand{}, finished{}, total_value{} {}

    HandHolder::~HandHolder() {}

    void HandHolder::startNewHand(Deck& deck) {
        hand.clear();
        hand.insert(hand.end(), { deck.takeCard(), deck.takeCard() });
        finished = false;
        calculateTotalValue();
    }

    int HandHolder::getTotalValue() const {
        return total_value;
    }

    bool HandHolder::isFinished() const {
        return finished;
    }

    void HandHolder::hit(Deck& deck) {
        hand.push_back(deck.takeCard());
        calculateTotalValue();
    }

    void HandHolder::stand() {
        finished = true;
    }

    bool HandHolder::hasAce() {
        return std::any_of(hand.begin(), hand.end(), [](const Card& card) {
            return card.face == "A";
        });
    }

    void HandHolder::calculateTotalValue() {
        total_value = 0;
        for (const auto& card : hand) {
            total_value += card.value;
        }
        if (hasAce() && total_value + 10 <= Blackjack) {
            total_value += 10;
        }
        if (total_value >= Blackjack) {
            finished = true;
        }
    }

    Player::Player() : HandHolder{} {
        // [=] capture by value captures '[this]'
        action_map['h'] = [=](Deck& deck) { hit(deck); };
        action_map['s'] = [=](Deck&) { stand(); };
        action_map['d'] = [=](Deck& deck) { doubleDown(deck); };
        action_map['p'] = [=](Deck&) { split(); };
    }

    void Player::processAction(char action, Deck& deck) {   
        action_map[action](deck);
    }

    std::string Player::getCardsString() {
        std::ostringstream ss;
        ss << "Player hand (" << getTotalValue() << "): ";
        for (const Card& card : hand) {
            ss << card.face << card.suit << ' ';
        }
        ss << '\n';
        return ss.str();
    }

    void Player::doubleDown(Deck& deck) {
        hit(deck);
        finished = true;
    }

    void Player::split() {
        // not implemented yet
    }

    Dealer::Dealer() : HandHolder{} {}

    void Dealer::playHand(int player_total, Deck& deck) {
        if (player_total > Blackjack) {
            finished = true;
            return;
        }
        while (!finished) {
            getTotalValue() < DealerStand ? hit(deck) : stand();
        }
    }

    std::string Dealer::getCardsString(bool player_turn_finished) {
        std::ostringstream ss;
        if (!player_turn_finished) {
            ss << "Dealer hand: " << hand[0].face << hand[0].suit << " ▮\n";
        } else {
            ss << "Dealer hand (" << getTotalValue() << "): ";
            for (const auto& card : hand) {
                ss << card.face << card.suit << ' ';
            }
            ss << '\n';
        }
        return ss.str();
    }

    Game::Game() : deck{}, dealer{}, player{} {}

    void Game::play() {
        while (true) {
            clearTerminal();
            if (deck.needsShuffle()) {
                std::cout << "Shuffling cards...\n";
                deck.shuffle();
            }
            player.startNewHand(deck);
            dealer.startNewHand(deck);
            playerTurn();
            dealerTurn();
            decideWinner();
        }
    }

    void Game::clearTerminal() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void Game::displayGameState() {
        std::cout << dealer.getCardsString(player.isFinished());
        std::cout << player.getCardsString();
    }

    bool Game::isValidInput() {
        bool input_correct{ std::cin };
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore everything up to '\n' and remove it as well
        return input_correct;
    }

    bool Game::isValidAction(char action) {
        return PossibleActions.find(action) != PossibleActions.npos;
    }

    void Game::handlePlayerInput() {
        char action;
        std::cout << "\nChoose your next action\n(h)it, (s)tand, (d)ouble down, s(p)lit: ";
        std::cin >> action;
        if (isValidInput() && isValidAction(action)) {
            std::cout << '\n';
            player.processAction(action, deck);
        } else {
            std::cout << "Incorrect input! Try again.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }
    }

    void Game::playerTurn() {
        while (!player.isFinished()) {
            displayGameState();
            handlePlayerInput();
            clearTerminal();
        }
    }

    void Game::dealerTurn() {
        dealer.playHand(player.getTotalValue(), deck);
        displayGameState();
    }

    void Game::decideWinner() {
        const int player_total{ player.getTotalValue() };
        const int dealer_total{ dealer.getTotalValue() };
        std::string result{};
        if (player_total > Blackjack) {
            result = "Dealer won! Player is over 21.";
        } else if (dealer_total > Blackjack) {
            result = "Player won! Dealer is over 21.";
        } else if (player_total > dealer_total) {
            result = "Player won!";
        } else if (dealer_total > player_total) {
            result = "Dealer won!";
        } else {
            result = "Draw!";
        }
        std::cout << result << "\n\n";
        std::cout << "Press Enter to continue "; // Let user observe win
        std::cin.get();
    }
}