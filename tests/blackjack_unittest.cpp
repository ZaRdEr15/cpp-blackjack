#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "blackjack.hpp"

TEST_GROUP(CardTestGroup) {
};

TEST(CardTestGroup, ConstructorInitializesCorrectly) {
    Blackjack::Card king_card {"K", "♥"};
    STRCMP_EQUAL("K", king_card.face.data()); // Necessary to check for only one object
    STRCMP_EQUAL("♥", king_card.suit.data());
    LONGS_EQUAL(10, king_card.value);

    Blackjack::Card queen_card {"Q", "♥"};
    LONGS_EQUAL(10, queen_card.value);

    Blackjack::Card jack_card {"J", "♥"};
    LONGS_EQUAL(10, jack_card.value);

    Blackjack::Card ten_card {"10", "♥"};
    LONGS_EQUAL(10, ten_card.value);

    Blackjack::Card ace_card {"A", "♥"};
    LONGS_EQUAL(1, ace_card.value);

    Blackjack::Card seven_card {"7", "♥"};
    LONGS_EQUAL(7, seven_card.value);
}

int main(int ac, char** av) {
    return CommandLineTestRunner::RunAllTests(ac, av);
}
