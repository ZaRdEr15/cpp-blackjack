#include <iostream>
#include <limits> // for std::numeric_limits
#include <sstream> // for std::stringstream
#include <thread> // for std::this_thread::sleep_for
#include <chrono> // for std::chrono::seconds
#include "blackjack.hpp"

namespace Blackjack {

    Card::Card(std::string_view f, std::string_view s) : face {f}, suit {s}, value {FaceToValue.at(std::string {f})} {}

    const std::unordered_map<std::string_view, int> Card::FaceToValue {
        {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}, {"10", 10}, {"J", 10}, {"Q", 10}, {"K", 10}, {"A", 1}
    };

    Deck::Deck() {}

    std::vector<Card> Deck::initialDeal() {
        std::vector<Card> initial_hand {takeCard(), takeCard()};
        return initial_hand;
    }

    Card Deck::takeCard() {
        Card card {deck.back()};
        deck.pop_back();
        return card;
    }

    void Deck::fillAndShuffleDeck() {
        if (deck.size() <= MinDeckSizeBeforeRefill) {
            if (!deck.empty()) { // To not show first time when deck is empty
                std::cout << "Shuffling cards... (Deck size: " << deck.size() << ")\n";
                deck.clear(); 
            }
            for (std::string_view suit : Suit) {
                for (std::string_view face : SingleSuitFaces) {
                    deck.push_back(Card {face, suit});
                }
            }
            std::shuffle(std::begin(deck), std::end(deck), mt);
        }
    }

    void Deck::showDeck() {
        std::cout << "Deck (" << deck.size() << "):" << '\n';
        for (const Card& card : deck) {
            std::cout << card.face << card.suit << ' ';
        }
    }

    HandHolder::HandHolder(std::vector<Card> initial_hand) :
        total_value {0}, finished {false}, hand {initial_hand}  {
        calculateTotalValue();
    }

    HandHolder::~HandHolder() {}

    void HandHolder::hit(Deck& deck_instance) {
        hand.push_back(deck_instance.takeCard());
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
        for (const Card& card : hand) {
            total_value += card.value;
        }
        if (hasAce() && total_value + 10 <= Blackjack) {
            total_value += 10;
        }
        if (total_value >= Blackjack) {
            finished = true;
        }
    }

    Player::Player(std::vector<Card> initial_hand) : HandHolder {initial_hand} {
        action_map['h'] = [=](Deck& deck_instance) { hit(deck_instance); }; // [=] capture by value captures '[this]'
        action_map['s'] = [=]([[maybe_unused]] Deck& deck_instance) { stand(); };
        action_map['d'] = [=](Deck& deck_instance) { doubleDown(deck_instance); };
        action_map['p'] = [=]([[maybe_unused]] Deck& deck_instance) { split(); };
    }

    void Player::processAction(const char& action, Deck& deck_instance) {   
        action_map[action](deck_instance);
    }

    std::string Player::getCardsString() {
        std::ostringstream ss;
        ss << "Player hand (" << total_value << "): ";
        for (const Card& card : hand) {
            ss << card.face << card.suit << ' ';
        }
        ss << '\n';
        return ss.str();
    }

    void Player::doubleDown(Deck& deck_instance) {
        hit(deck_instance);
        finished = true;
    }

    void Player::split() {
        // not implemented yet
    }

    Dealer::Dealer(std::vector<Card> initial_hand) : HandHolder {initial_hand} {}

    void Dealer::playHand(const int& player_total, Deck& deck_instance) {
        if (player_total > Blackjack) {
            finished = true;
            return;
        }
        while (!finished) {
            total_value < DealerStand ? hit(deck_instance) : stand();
        }
    }

    std::string Dealer::getCardsString(const bool& player_turn_finished) {
        std::ostringstream ss;
        if (!player_turn_finished) {
            ss << "Dealer hand: " << hand[0].face << hand[0].suit << " ▮\n";
        } else {
            ss << "Dealer hand (" << total_value << "): ";
            for (const Card& card : hand) {
                ss << card.face << card.suit << ' ';
            }
            ss << '\n';
        }
        return ss.str();
    }

    Game::Game() : deck {} {}

    void Game::play() {
        while (true) {
            clearTerminal();
            deck.fillAndShuffleDeck();
            Dealer dealer {deck.initialDeal()};
            Player player {deck.initialDeal()};
            playerTurn(dealer, player);
            dealerTurn(dealer, player);
            decideWinner(player, dealer);
        }
    }

    void Game::clearTerminal() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void Game::displayGameState(Dealer& dealer_instance, Player& player_instance) {
        std::cout << dealer_instance.getCardsString(player_instance.finished);
        std::cout << player_instance.getCardsString();
    }

    bool Game::isValidInput() {
        bool input_correct {std::cin};
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore everything up to '\n' and remove it as well
        return input_correct;
    }

    bool Game::isValidAction(const char& action) {
        return (PossibleActions.find(action) != PossibleActions.npos);
    }

    void Game::handlePlayerInput(Player& player_instance) {
        char action;
        std::cout << "\nChoose your next action\n(h)it, (s)tand, (d)ouble down, s(p)lit: ";
        std::cin >> action;
        if (isValidInput() && isValidAction(action)) {
            std::cout << '\n';
            player_instance.processAction(action, deck);
        } else {
            std::cout << "Incorrect input! Try again.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }
    }

    void Game::playerTurn(Dealer& dealer_instance, Player& player_instance) {
        while (!player_instance.finished) {
            displayGameState(dealer_instance, player_instance);
            handlePlayerInput(player_instance);
            clearTerminal();
        }
    }

    void Game::dealerTurn(Dealer& dealer_instance, Player& player_instance) {
        dealer_instance.playHand(player_instance.total_value, deck);
        displayGameState(dealer_instance, player_instance);
    }

    void Game::decideWinner(const Player& player, const Dealer& dealer) {
        if (player.total_value > Blackjack) {
            std::cout << "Dealer won! Player is over 21.\n\n";
        } else if (dealer.total_value > Blackjack) {
            std::cout << "Player won! Dealer is over 21.\n\n";
        } else if (player.total_value > dealer.total_value) {
            std::cout << "Player won!\n\n";
        } else if (dealer.total_value > player.total_value) {
            std::cout << "Dealer won!\n\n";
        } else if (player.total_value == dealer.total_value) {
            std::cout << "Draw!\n\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}