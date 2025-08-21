#include <iostream>
#include "blackjack.hpp"

namespace Blackjack {

    Card::Card(std::string_view f, std::string_view s) : face {f}, suit {s} {
        faceToValue(f);
    }

    void Card::faceToValue(std::string_view f) {
        if (f == "J" || f == "Q" || f == "K" || f == "10") {
            value = 10;
        } else if (f == "A") {
            value = 1;
        } else {
            value = f[0] - '0';
        }
    }

    HandHolder::HandHolder(std::vector<Card> initial_hand) :
        total_value {0}, hand {initial_hand}, finished {false}  {
        calculateTotalValue();
    }

    void HandHolder::showCards() {}

    void HandHolder::hit(Game& game_instance) {
        hand.push_back(game_instance.takeCard());
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

    Player::Player(std::vector<Card> initial_hand) : HandHolder {initial_hand} {}

    void Player::showCards() {
        std::cout << "Player hand (" << total_value << "):\n";
        for (const auto& card : hand) {
            std::cout << card.face << card.suit << '\n';
        }
    }

    void Player::chooseAction(Game& game_instance) {
        while (!finished) {
            char action;
            std::cout << "Choose your next action\n(h)it, (s)tand, (d)ouble down, s(p)lit: ";
            std::cin >> action;
            switch (action) {
                case 'h':
                    hit(game_instance);
                    showCards();
                    break;
                case 's':
                    stand();
                    showCards();
                    break;
                case 'd':
                    doubleDown(game_instance);
                    showCards();
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

    void Player::doubleDown(Game& game_instance) {
        hit(game_instance);
        finished = true;
    }

    void Player::split() {
        // not implemented yet
    }

    Dealer::Dealer(std::vector<Card> initial_hand) : HandHolder {initial_hand}, show_once {true} {}

    void Dealer::playHand(const int& player_total, Game& game_instance) {
        if (player_total > Blackjack) {
            finished = true;
            return;
        }
        while (!finished) {
            total_value < DealerStand ? hit(game_instance) : stand();
        }
    }

    void Dealer::showCards() {
        if (!finished && show_once) {
            show_once = false;
            std::cout << "Dealer hand:" << '\n'
                      << hand[0].face << hand[0].suit << '\n'
                      << "X" << '\n';
        } else {
            show_once = true;
            std::cout << "Dealer hand (" << total_value << "):\n";
            for (const auto& card : hand) {
                std::cout << card.face << card.suit << '\n';
            }
        }
    }

    Game::Game() {}

    void Game::play() {
        while (true) {
            fillDeck();
            Player player {initialDeal()};
            Dealer dealer {initialDeal()};
            dealer.showCards();
            player.showCards();
            player.chooseAction(*this);
            dealer.playHand(player.total_value, *this);
            dealer.showCards();
            decideWinner(player, dealer);
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

    void Game::fillDeck() {
        if (deck.size() < RefillDeck) {
            if (deck.size() == 0) {
                std::cout << "Shuffling cards... (Deck size: " << deck.size() << ")\n";
            }
            if (!deck.empty()) { 
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

    void Game::showDeck() {
        std::cout << "Deck (" << deck.size() << "):" << '\n';
        for (const auto& card : deck) {
            std::cout << card.face << card.suit << ' ';
        }
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
    }
}