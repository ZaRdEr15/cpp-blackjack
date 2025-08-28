#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(FirstTestGroup) {
    void setup() {

    }

    void teardown() {

    }
};

TEST(FirstTestGroup, FirstTest) {
   FAIL("Fail me!");
}

TEST(FirstTestGroup, SecondTest) {
    STRCMP_EQUAL("hello", "world");
}

int main(int ac, char** av) {
    return CommandLineTestRunner::RunAllTests(ac, av);
}
