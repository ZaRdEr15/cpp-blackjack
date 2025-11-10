#include <iostream>
#include <limits>   // std::numeric_limits
#include <sstream>  // std::stringstream
#include <thread>   // std::this_thread::sleep_for
#include <chrono>   // std::chrono::seconds
#include "blackjack.hpp"

namespace Blackjack {

    Card::Card(std::string_view face, 
               std::string_view suit) : m_face{ face }, m_suit{ suit } {}

    std::string_view Card::getFace() const {
        return m_face;
    }

    std::string_view Card::getSuit() const {
        return m_suit;
    }

    int Card::getValue() const {
        return kFaceToValue.at(m_face);
    }

    Deck::Deck() {
        shuffle();
    }

    Card Deck::takeCard() {
        Card card{ m_cards.back() };
        m_cards.pop_back();
        return card;
    }

    bool Deck::needsShuffle() const {
        return m_cards.size() <= kMinDeckSizeBeforeRefill;
    }

    void Deck::shuffle() {
        m_cards.clear();
        for (auto suit : kSuit) {
            for (auto face : kSingleSuitFaces) {
                m_cards.push_back(Card{ face, suit });
            }
        }
        std::shuffle(m_cards.begin(), m_cards.end(), g_mt);
    }

    void Deck::showDeck() const {
        std::cout << "Deck (" << m_cards.size() << "):" << '\n';
        for (const auto& card : m_cards) {
            std::cout << card.getFace() << card.getSuit() << ' ';
        }
    }

    HandHolder::HandHolder() : m_hand{}, m_finished{}, m_total_value{} {}

    HandHolder::~HandHolder() {}

    void HandHolder::startNewHand(Deck& deck) {
        m_hand.clear();
        m_hand.insert(m_hand.end(), { deck.takeCard(), deck.takeCard() });
        m_finished = false;
        calculateTotalValue();
    }

    int HandHolder::getTotalValue() const {
        return m_total_value;
    }

    bool HandHolder::isFinished() const {
        return m_finished;
    }

    void HandHolder::hit(Deck& deck) {
        m_hand.push_back(deck.takeCard());
        calculateTotalValue();
    }

    void HandHolder::stand() {
        m_finished = true;
    }

    bool HandHolder::hasAce() const {
        return std::any_of(m_hand.begin(), m_hand.end(), [](const Card& card) {
            return card.getFace() == "A";
        });
    }

    void HandHolder::calculateTotalValue() {
        m_total_value = 0;
        for (const auto& card : m_hand) {
            m_total_value += card.getValue();
        }
        if (hasAce() && m_total_value + 10 <= kBlackjack) {
            m_total_value += 10;
        }
        if (m_total_value >= kBlackjack) {
            m_finished = true;
        }
    }

    Player::Player() : HandHolder{} {
        // [=] capture by value captures '[this]'
        m_action_map['h'] = [=](Deck& deck) { hit(deck); };
        m_action_map['s'] = [=](Deck&) { stand(); };
        m_action_map['d'] = [=](Deck& deck) { doubleDown(deck); };
        m_action_map['p'] = [=](Deck&) { split(); };
    }

    void Player::processAction(char action, Deck& deck) {   
        m_action_map[action](deck);
    }

    const std::string& Player::getCardsStr() const {
        std::ostringstream ss;
        ss << "Player hand (" << getTotalValue() << "): ";
        for (const Card& card : m_hand) {
            ss << card.getFace() << card.getSuit() << ' ';
        }
        ss << '\n';
        return ss.str();
    }

    void Player::doubleDown(Deck& deck) {
        hit(deck);
        m_finished = true;
    }

    void Player::split() {
        // not implemented yet
    }

    Dealer::Dealer() : HandHolder{} {}

    void Dealer::playHand(int player_total, Deck& deck) {
        if (player_total > kBlackjack) {
            m_finished = true;
            return;
        }
        while (!m_finished) {
            getTotalValue() < kDealerStand ? hit(deck) : stand();
        }
    }

    const std::string& Dealer::getCardsStr(bool player_turn_finished) const {
        std::ostringstream ss;
        if (!player_turn_finished) {
            ss << "Dealer hand: " << m_hand[0].getFace() << m_hand[0].getSuit() << " ▮\n";
        } else {
            ss << "Dealer hand (" << getTotalValue() << "): ";
            for (const auto& card : m_hand) {
                ss << card.getFace() << card.getSuit() << ' ';
            }
            ss << '\n';
        }
        return ss.str();
    }

    Game::Game() : m_deck{}, m_dealer{}, m_player{} {}

    void Game::play() {
        while (true) {
            clearTerminal();
            if (m_deck.needsShuffle()) {
                std::cout << "Shuffling cards...\n";
                m_deck.shuffle();
            }
            m_player.startNewHand(m_deck);
            m_dealer.startNewHand(m_deck);
            playerTurn();
            dealerTurn();
            decideWinner();
        }
    }

    void Game::clearTerminal() const {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void Game::displayGameState() const {
        std::cout << m_dealer.getCardsStr(m_player.isFinished());
        std::cout << m_player.getCardsStr();
    }

    bool Game::isValidInput() {
        bool input_correct{ std::cin };
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore everything up to '\n' and remove it as well
        return input_correct;
    }

    bool Game::isValidAction(char action) const {
        return kPossibleActions.find(action) != kPossibleActions.npos;
    }

    void Game::handlePlayerInput() {
        char action;
        std::cout << "\nChoose your next action\n(h)it, (s)tand, (d)ouble down, s(p)lit: ";
        std::cin >> action;
        if (isValidInput() && isValidAction(action)) {
            std::cout << '\n';
            m_player.processAction(action, m_deck);
        } else {
            std::cout << "Incorrect input! Try again.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
        }
    }

    void Game::playerTurn() {
        while (!m_player.isFinished()) {
            displayGameState();
            handlePlayerInput();
            clearTerminal();
        }
    }

    void Game::dealerTurn() {
        m_dealer.playHand(m_player.getTotalValue(), m_deck);
        displayGameState();
    }

    void Game::decideWinner() const {
        const int player_total{ m_player.getTotalValue() };
        const int dealer_total{ m_dealer.getTotalValue() };
        std::string result{};
        if (player_total > kBlackjack) {
            result = "Dealer won! Player is over 21.";
        } else if (dealer_total > kBlackjack) {
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