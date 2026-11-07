#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "lib/doctest.h"
#include "trains.h"

TEST_CASE("dummy_fn") {
    SUBCASE("returns 123") {
        CHECK(dummy_fn() == 123);
    }
}
