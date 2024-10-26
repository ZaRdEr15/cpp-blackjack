#include <iostream>
#include "blackjack.hpp"

namespace Blackjack {

    std::vector<Card> deck;

    void Game::play() {
        auto rd {std::random_device {}};
        auto rng {std::default_random_engine { rd() }};
        fillDeck();
        //showDeck(); // DEBUG
        shuffleDeck(rng);
        HandHolder player {takeCard(), takeCard()};
        player.showCards();
    }

    void Game::fillDeck() {
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
        for (const auto& card : deck) {
            std::cout << card.face << card.suit << '\n';
        }
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

    HandHolder::HandHolder(Card first_card, Card second_card) {
        hand.push_back(first_card);
        hand.push_back(second_card);
        finished = false;
        calculateTotalValue();
    }

    void HandHolder::hit() {
        //takeCard();
    }

    void HandHolder::stand() {

    }

    void HandHolder::calculateTotalValue() {
        for (const auto& card : hand) {
            total_value += card.value;
        }
    }

    void HandHolder::showCards() {
        std::cout << "HandHolder hand (" << total_value << "):" << '\n';
        for (const auto& card : hand) {
            std::cout << card.face << card.suit << '\n';
        }
    }
}