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

TEST_CASE("Wagon") {
    SUBCASE("correctly created from wagon series") {
        Wagon::unique_id_t init_num = Wagon::ever_built();
        size_t init_in_service = Wagon::in_service();

        WagonSeries series(WagonType::DINING, 100);
        Wagon wagon(series);
        CHECK(wagon.number() == init_num + 1);
        CHECK(wagon.series() == series);

        CHECK(Wagon::ever_built() == init_num + 1);     // Ensure counters increment once
        CHECK(Wagon::in_service() == init_in_service + 1);
    }

    SUBCASE("wagons are numbered correctly") {
        Wagon::unique_id_t init_num = Wagon::ever_built();
        Wagon w1(STANDARD_FIRST_CLASS);
        Wagon w2(STANDARD_FIRST_CLASS);
        Wagon w3(STANDARD_FIRST_CLASS);
        Wagon w4(STANDARD_FIRST_CLASS);
        CHECK(w1.number() == init_num + 1);
        CHECK(w2.number() == init_num + 2);
        CHECK(w3.number() == init_num + 3);
        CHECK(w4.number() == init_num + 4);
    }

    SUBCASE("empty wagon created correctly") {
        Wagon::unique_id_t init_num = Wagon::ever_built();
        size_t init_in_service = Wagon::in_service();

        Wagon wagon;
        CHECK(wagon.number() == 0);
        CHECK(!wagon);     // Check that wagon is empty

        CHECK(Wagon::ever_built() == init_num);     // Ensure counters don't change
        CHECK(Wagon::in_service() == init_in_service);
    }

    SUBCASE("copying constructor works correctly") {
        Wagon w1(STANDARD_FIRST_CLASS);
        Wagon::unique_id_t init_num = Wagon::ever_built();
        size_t init_in_service = Wagon::in_service();

        Wagon w2(w1);
        CHECK(w2.number() == init_num + 1);
        CHECK(Wagon::ever_built() == init_num + 1);  // Ensure counters increment once
        CHECK(Wagon::in_service() == init_in_service + 1);

        Wagon empty_wagon;
        Wagon w4(empty_wagon);
        CHECK(!w4);
        CHECK(Wagon::ever_built() == init_num + 1);     // Ensure counters don't change
        CHECK(Wagon::in_service() == init_in_service + 1);
    }

    SUBCASE("move constructor works correctly") {
        Wagon::unique_id_t ever_built_old = Wagon::ever_built();
        size_t in_service_old = Wagon::in_service();

        Wagon w1(STANDARD_FIRST_CLASS);

        REQUIRE(Wagon::ever_built() == ever_built_old + 1);
        REQUIRE(Wagon::in_service() == in_service_old + 1);

        Wagon w2(std::move(w1));
        CHECK(!w1);
        CHECK(w2.series() == STANDARD_FIRST_CLASS);

        CHECK(Wagon::ever_built() == ever_built_old + 1);   // Ensure counters don't change
        CHECK(Wagon::in_service() == in_service_old + 1);
    }

    SUBCASE("copy assignment works correctly") {
        SUBCASE("wagon = wagon") {
            Wagon a(STANDARD_FIRST_CLASS);
            Wagon b(STANDARD_SECOND_CLASS);
            Wagon::unique_id_t old_number_a = a.number();
            Wagon::unique_id_t old_number_b = b.number();

            Wagon::unique_id_t old_last_number = Wagon::ever_built();
            size_t old_in_service = Wagon::in_service();

            a = b;
            CHECK(a.number() == old_number_a);  // Ensure a kept its number
            CHECK(a.series() == STANDARD_SECOND_CLASS);
            CHECK(b.number() == old_number_b);  // b still exists
            CHECK(b.series() == STANDARD_SECOND_CLASS);

            CHECK(Wagon::ever_built() == old_last_number);   // Verify counters after assignment
            CHECK(Wagon::in_service() == old_in_service);
        }

        SUBCASE("empty wagon = wagon") {
            Wagon a = Wagon();
            Wagon b = Wagon(STANDARD_SLEEPER);
            Wagon::unique_id_t old_number_b = b.number();

            Wagon::unique_id_t old_last_number = Wagon::ever_built();
            size_t old_in_service = Wagon::in_service();

            a = b;
            CHECK(a.number() == old_last_number + 1);
            CHECK(a.series() == STANDARD_SLEEPER);
            CHECK(b.number() == old_number_b);  // b still exists
            CHECK(b.series() == STANDARD_SLEEPER);

            CHECK(Wagon::ever_built() == old_last_number + 1);   // Verify counters after assignment
            CHECK(Wagon::in_service() == old_in_service + 1);
        }

        SUBCASE("wagon = empty wagon") {
            Wagon a = Wagon(STANDARD_DINING);
            Wagon b = Wagon();

            Wagon::unique_id_t old_last_number = Wagon::ever_built();
            size_t old_in_service = Wagon::in_service();

            a = b;
            CHECK(!a);
            CHECK(!b);

            CHECK(Wagon::ever_built() == old_last_number);       // Verify counters after assignment
            CHECK(Wagon::in_service() == old_in_service - 1);
        }

        SUBCASE("empty wagon = empty wagon") {
            Wagon a = Wagon();
            Wagon b = Wagon();

            Wagon::unique_id_t old_last_number = Wagon::ever_built();
            size_t old_in_service = Wagon::in_service();

            a = b;
            CHECK(!a);
            CHECK(!b);

            CHECK(Wagon::ever_built() == old_last_number);       // Verify counters after assignment
            CHECK(Wagon::in_service() == old_in_service);
        }

        SUBCASE("self-assignment") {
            Wagon wagon(STANDARD_FIRST_CLASS);
            Wagon::unique_id_t old_number = wagon.number();
            WagonSeries old_series = wagon.series();

            wagon = wagon;
            CHECK(wagon.number() == old_number);
            CHECK(wagon.series() == old_series);
        }
    }

    SUBCASE("move assignment works correctly") {
        SUBCASE("wagon = wagon") {
            Wagon a(STANDARD_FIRST_CLASS);
            Wagon b(STANDARD_SECOND_CLASS);
            Wagon::unique_id_t old_number_b = b.number();

            Wagon::unique_id_t old_last_number = Wagon::ever_built();
            size_t old_in_service = Wagon::in_service();

            a = std::move(b);
            CHECK(a.number() == old_number_b);
            CHECK(a.series() == STANDARD_SECOND_CLASS);
            CHECK(!b);

            CHECK(Wagon::ever_built() == old_last_number);      // Verify counters after assignment
            CHECK(Wagon::in_service() == old_in_service - 1);   // (a was made empty)
        }

        SUBCASE("empty wagon = wagon") {
            Wagon a = Wagon();
            Wagon b = Wagon(STANDARD_SLEEPER);
            Wagon::unique_id_t old_number_b = b.number();

            Wagon::unique_id_t old_last_number = Wagon::ever_built();
            size_t old_in_service = Wagon::in_service();

            a = std::move(b);
            CHECK(a.number() == old_number_b);
            CHECK(a.series() == STANDARD_SLEEPER);
            CHECK(!b);

            CHECK(Wagon::ever_built() == old_last_number);  // Verify counters after assignment
            CHECK(Wagon::in_service() == old_in_service);
        }

        SUBCASE("wagon = empty wagon") {
            Wagon a = Wagon(STANDARD_DINING);
            Wagon b = Wagon();

            Wagon::unique_id_t old_last_number = Wagon::ever_built();
            size_t old_in_service = Wagon::in_service();

            a = std::move(b);
            CHECK(!a);
            CHECK(!b);

            CHECK(Wagon::ever_built() == old_last_number);      // Verify counters after assignment
            CHECK(Wagon::in_service() == old_in_service - 1);   // (a was made empty)
        }

        SUBCASE("empty wagon = empty wagon") {
            Wagon a = Wagon();
            Wagon b = Wagon();

            Wagon::unique_id_t old_last_number = Wagon::ever_built();
            size_t old_in_service = Wagon::in_service();

            a = std::move(b);

            CHECK(!a);
            CHECK(!b);

            CHECK(Wagon::ever_built() == old_last_number);      // Verify counters after assignment
            CHECK(Wagon::in_service() == old_in_service);
        }

        SUBCASE("self-assignment") {
            Wagon wagon(STANDARD_FIRST_CLASS);
            Wagon::unique_id_t old_number = wagon.number();
            WagonSeries old_series = wagon.series();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
            wagon = std::move(wagon);
#pragma GCC diagnostic pop
            CHECK(wagon.number() == old_number);
            CHECK(wagon.series() == old_series);
        }
    }

    SUBCASE("allow implicit conversion to WagonSeries") {
        Wagon wagon = Wagon(STANDARD_DINING);
        WagonSeries series = wagon;
        CHECK(series == STANDARD_DINING);
    }

    SUBCASE("comparison operators work") {
        Wagon w1(STANDARD_FIRST_CLASS);
        Wagon w2(STANDARD_SECOND_CLASS);

        CHECK(w1 < w2);
        CHECK(w1 <= w2);
        CHECK(w2 > w1);
        CHECK(w2 >= w1);
        CHECK((w1 <=> w2) == std::strong_ordering::less);

        CHECK(w1 != w2);
        CHECK(w1 == w1);
        CHECK(w2 == w2);

        Wagon w3;
        Wagon w4;

        CHECK(w3 < w1);
        CHECK(w3 < w2);
        CHECK(w3 == w3);
        CHECK(w3 == w4);
    }

    SUBCASE("correctly prints out onto ostream") {
        std::stringstream os;

        Wagon wagon(STANDARD_FIRST_CLASS);
        Wagon empty_wagon;
        os << wagon << empty_wagon;

        std::stringstream expected_os;
        expected_os << "Wagon nr " << wagon.number() << " [1 klasa, miejsca: 54]";
        expected_os << "Wagon pusty";

        CHECK(os.str() == expected_os.str());
    }
}

TEST_CASE("Locomotive") {
    SUBCASE("correctly created from params") {
        Locomotive loco("ET26-006", 1'000);
        CHECK(loco.designation() == "ET26-006");
        CHECK(loco.max_wagons() == 1'000);

        Locomotive unnamed_loco("", 99);    // Check that the name is '?' if provided string was empty
        CHECK(unnamed_loco.designation() == "?");
    }

    SUBCASE("correctly creates empty locomotive") {
        Locomotive empty_loco;
        CHECK(empty_loco.designation() == "");
        CHECK(empty_loco.max_wagons() == 0);
        CHECK(!empty_loco);
    }

    SUBCASE("move constructor works properly") {
        Locomotive et26("ET26-006", 100);
        Locomotive ep05(std::move(et26));

        CHECK(ep05.designation() == "ET26-006");
        CHECK(ep05.max_wagons() == 100);
        CHECK(!et26);
    }

    SUBCASE("move assignment works properly") {
        Locomotive et26("ET26-006", 100);
        Locomotive ep05("EP05-001", 200);

        et26 = std::move(ep05);
        CHECK(et26.designation() == "EP05-001");
        CHECK(et26.max_wagons() == 200);
        CHECK(!ep05);

        SUBCASE("self-assignment") {
            Locomotive en57akm("EN57AKM-1601", 1);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
            en57akm = std::move(en57akm);
#pragma GCC diagnostic pop

            CHECK(en57akm.designation() == "EN57AKM-1601");
            CHECK(en57akm.max_wagons() == 1);
        }
    }

    SUBCASE("cannot copy locomotives") {
        static_assert(!std::is_copy_assignable_v<Locomotive>);
        static_assert(!std::is_copy_constructible_v<Locomotive>);
    }

    SUBCASE("correctly prints out onto ostream") {
        std::stringstream os;
        Locomotive loco("ET26-006", 1'000);
        Locomotive empty_loco;
        os << loco << empty_loco;

        std::string expected_str;
        expected_str += "ET26-006 (uciąg: 1000)";
        expected_str += "brak lokomotywy";

        CHECK(os.str() == expected_str);
    }
}
