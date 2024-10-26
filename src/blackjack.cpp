#include <iostream>
#include "blackjack.hpp"

namespace Blackjack {

    std::vector<Card> deck;

    void Game::play() {
        auto rd {std::random_device {}};
        auto rng {std::default_random_engine { rd() }};
        fillDeck();
        shuffleDeck(rng);
        Player player {initialDeal()};
        //showDeck(); // DEBUG
    }

    void Game::fillDeck() {
        if (!deck.empty()) { deck.clear(); }
        std::array<std::string, SUITS> suits {"♣", "♦", "♥", "♠"};
        std::array<std::string, SUIT_SIZE> single_suit_faces {
            "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
        };
        for (const auto& suit : suits) {
            for (const auto& face : single_suit_faces) {
                Card card {face, suit};
                deck.push_back(card);
            }
        }
    }

    void Game::shuffleDeck(std::default_random_engine rng) {
        std::shuffle(std::begin(deck), std::end(deck), rng);
    }

    void Game::showDeck() {
        std::cout << "Deck (" << deck.size() << "):" << '\n';
        for (const auto& card : deck) {
            std::cout << card.face << card.suit << '\n';
        }
    }

    std::vector<Card> Game::initialDeal() {
        std::vector<Card> initial_hand {takeCard(), takeCard()};
        return initial_hand;
    }

    Card Game::takeCard() {
        Card card {deck.back()};
        deck.pop_back();
        return card;
    }

    Card::Card(const std::string f, const std::string s) {
        ace = f == "A" ? true : false;
        face = f;
        suit = s;
        faceToValue(f);
    }

    void Card::faceToValue(const std::string f) {
        if (f == "J" || f == "Q" || f == "K" || f == "10") {
            value = 10;
        } else if (f == "A") {
            value = 11;
        } else {
            value = f[0] - 48;
        }
    }

    HandHolder::HandHolder(std::vector<Card> initial_hand) {
        hand = initial_hand;
        finished = false;
        calculateTotalValue();
    }

    void HandHolder::hit() {
        Card new_card {Game::takeCard()};
        hand.push_back(new_card);
        total_value += new_card.value;
    }

    void HandHolder::stand() {

    }

    void HandHolder::calculateTotalValue() {
        for (const auto& card : hand) {
            total_value += card.value;
        }
    }
    
    void HandHolder::showCards() {}

    Player::Player(std::vector<Card> initial_hand) : HandHolder(initial_hand) {}

    void Player::showCards() {
        std::cout << "Player hand (" << total_value << "):" << '\n';
        for (const auto& card : hand) {
            std::cout << card.face << card.suit << '\n';
        }
    }
}