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
        player.showCards();
        player.chooseAction();
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
        total_value = 0;
        calculateTotalValue();
    }

    void HandHolder::hit() {
        Card new_card {Game::takeCard()};
        hand.push_back(new_card);
        total_value += new_card.face == "A" && hasAce() ? 1 : new_card.value;
        if (total_value > BLACKJACK) {
            finished = true;
            std::cout << "Over 21!" << '\n';
        }
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
        for (const auto& card : hand) {
            total_value += card.value;
        }
        total_value -= total_value > BLACKJACK ? 10 : 0;
    }
    
    void HandHolder::showCards() {}

    Player::Player(std::vector<Card> initial_hand) : HandHolder(initial_hand) {}

    void Player::doubleDown() {
        hit();
        finished = true;
    }

    void Player::split() {

    }

    void Player::showCards() {
        std::cout << "Player hand (" << total_value << "):" << '\n';
        for (const auto& card : hand) {
            std::cout << card.face << card.suit << '\n';
        }
    }

    void Player::chooseAction() {
        while (!finished) {
            char action;
            std::cout << "Choose your next action\n(h)it, (s)tand, (d)ouble down, s(p)lit: ";
            std::cin >> action;
            switch (action) {
                case 'h':
                    hit();
                    break;
                case 's':
                    stand();
                    break;
                case 'd':
                    doubleDown();
                    break;
                case 'p':
                    split();
                    break;
                default:
                    std::cout << "Incorrect input! Try again.\n";
                    continue;
            }
        }
    }
}