#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "blackjack.hpp"

TEST_GROUP(CardTestGroup) {
};

TEST(CardTestGroup, ConstructorInitializesCorrectly) {
    Blackjack::Card king_card{ "K", "♥" };
    STRCMP_EQUAL("K", king_card.getFace().data()); // Necessary to check for only one object
    STRCMP_EQUAL("♥", king_card.getSuit().data());
    LONGS_EQUAL(10, king_card.getValue());

    Blackjack::Card queen_card{ "Q", "♥" };
    LONGS_EQUAL(10, queen_card.getValue());

    Blackjack::Card jack_card{ "J", "♥" };
    LONGS_EQUAL(10, jack_card.getValue());

    Blackjack::Card ten_card{ "10", "♥" };
    LONGS_EQUAL(10, ten_card.getValue());

    Blackjack::Card ace_card{ "A", "♥" };
    LONGS_EQUAL(1, ace_card.getValue());

    Blackjack::Card seven_card{ "7", "♥" };
    LONGS_EQUAL(7, seven_card.getValue());
}

int main(int ac, char** av) {
    return CommandLineTestRunner::RunAllTests(ac, av);
}
