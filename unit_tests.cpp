#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "lib/doctest.h"
#include "trains.h"

TEST_CASE("WagonSeries") {
    SUBCASE("properly created from wagon type") {
        WagonSeries ws(WagonType::SECOND_CLASS);
        CHECK(ws.type() == WagonType::SECOND_CLASS);
        CHECK(ws.seats() == 80);
    }

    SUBCASE("properly created from cast") {
        WagonSeries ws = WagonType::DINING;
        CHECK(ws.type() == WagonType::DINING);
        CHECK(ws.seats() == 32);
    }

    SUBCASE("properly created with two params") {
        WagonSeries ws1(WagonType::FIRST_CLASS, 120);
        CHECK(ws1.type() == WagonType::FIRST_CLASS);
        CHECK(ws1.seats() == 120);

        WagonSeries ws2(WagonType::SECOND_CLASS, 200);
        CHECK(ws2.type() == WagonType::SECOND_CLASS);
        CHECK(ws2.seats() == 200);
    }

    SUBCASE("empty constructor doesn't exist") {
        static_assert(!std::is_default_constructible_v<WagonSeries>);
    }

    SUBCASE("copying constructor works") {
        WagonSeries ws1(WagonType::FIRST_CLASS);
        WagonSeries ws2(ws1);
        CHECK(ws1 == ws2);
    }

    SUBCASE("moving constructor works") {
        WagonSeries ws1(WagonType::FIRST_CLASS);
        WagonSeries ws2(std::move(ws1));
        CHECK(ws1 == ws2);
    }

    SUBCASE("copy assignment works") {
        WagonSeries ws1(WagonType::FIRST_CLASS);
        WagonSeries ws2(WagonType::SECOND_CLASS);
        ws1 = ws2;
        CHECK(ws1 == ws2);
    }

    SUBCASE("move assignment works") {
        WagonSeries ws1(WagonType::FIRST_CLASS);
        WagonSeries ws2(WagonType::SECOND_CLASS);
        ws1 = std::move(ws2);
        CHECK(ws1 == ws2);
    }

    SUBCASE("comparing works") {
        WagonSeries ws1(WagonType::FIRST_CLASS);
        WagonSeries ws2(WagonType::SECOND_CLASS);
        WagonSeries ws3(WagonType::SECOND_CLASS);
        CHECK(ws1 != ws2);
        CHECK(ws1 != ws3);
        CHECK(ws2 == ws3);

        WagonSeries ws4(WagonType::FIRST_CLASS, 60);
        WagonSeries ws5(WagonType::SECOND_CLASS, 60);
        WagonSeries ws6(WagonType::SECOND_CLASS, 70);
        CHECK(ws4 != ws5);
        CHECK(ws5 != ws6);
        CHECK(ws6 != ws4);
    }

    SUBCASE("correctly prints out onto ostream") {
        std::stringstream os;
        WagonSeries ws1(WagonType::FIRST_CLASS, 120);
        WagonSeries ws2(WagonType::SECOND_CLASS, 200);
        WagonSeries ws3(WagonType::SLEEPER, 999);
        WagonSeries ws4(WagonType::DINING, 500);
        os << ws1 << ws2 << ws3 << ws4;

        std::string expected_str;
        expected_str += "[1 klasa, miejsca: 120]";  // ws1
        expected_str += "[2 klasa, miejsca: 200]";  // ws2
        expected_str += "[sypialny, miejsca: 999]"; // ws3
        expected_str += "[restauracyjny, miejsca: 500]"; // ws4

        CHECK(os.str() == expected_str);
    }

    SUBCASE("can be used in constexpr expressions") {
        constexpr WagonSeries ws1(WagonType::FIRST_CLASS);
        constexpr WagonSeries ws2(WagonType::SECOND_CLASS, 200);
        static_assert(ws1.seats() == 54);
        static_assert(ws1.type() == WagonType::FIRST_CLASS);
        static_assert(ws2.seats() == 200);
        static_assert(ws2.type() == WagonType::SECOND_CLASS);
        static_assert(ws1 != ws2);
    }
}

TEST_CASE("standard wagon series") {
    SUBCASE("properly initialized") {
        CHECK(STANDARD_FIRST_CLASS.type() == WagonType::FIRST_CLASS);
        CHECK(STANDARD_FIRST_CLASS.seats() == 54);

        CHECK(STANDARD_SECOND_CLASS.type() == WagonType::SECOND_CLASS);
        CHECK(STANDARD_SECOND_CLASS.seats() == 80);

        CHECK(STANDARD_SLEEPER.type() == WagonType::SLEEPER);
        CHECK(STANDARD_SLEEPER.seats() == 30);

        CHECK(STANDARD_DINING.type() == WagonType::DINING);
        CHECK(STANDARD_DINING.seats() == 32);
    }
}
