/**
 * Kurs programowania w C++, 2026
 * Przykład do zadania nr 3 ,,Pociągi''
 * Plik:        trains_example.cpp
 * Autor:       Jakub Piotrowicz
 * Utworzono:   27.10.2026
 */

#include "../trains.h"

#ifdef NDEBUG
    #undef NDEBUG
#endif

#include <cassert>
#include <cstddef>
#include <iostream>
#include <utility>

using std::cout;

namespace {
    void series_examples() {
        constexpr WagonSeries compact{WagonType::SECOND_CLASS, 60};
        static_assert(compact.seats() == 60);
        static_assert(compact != STANDARD_SECOND_CLASS);

        // Niejawnie konwertujemy typ wagonu na serię standardową.
        constexpr WagonSeries sleeper = WagonType::SLEEPER;
        static_assert(sleeper == STANDARD_SLEEPER);

        cout << compact << '\n' << STANDARD_DINING << "\n\n";
    }

    void wagon_examples() {
        assert(Wagon::in_service() == 0);
        assert(Wagon::ever_built() == 0);

        Wagon w1{STANDARD_FIRST_CLASS};
        assert(w1.number() == 1);
        assert(w1.series() == STANDARD_FIRST_CLASS);

        // Kopiowanie buduje nowy wagon tej samej serii.
        Wagon w2{w1};
        assert(w2.number() == 2);
        assert(w2.series() == w1.series());
        assert(Wagon::in_service() == 2);
        assert(Wagon::ever_built() == 2);

        // Przenoszenie zachowuje numer, a obiekt źródłowy staje się pusty.
        Wagon w3{std::move(w2)};
        assert(w3.number() == 2);
        assert(!w2);
        assert(Wagon::in_service() == 2);
        assert(Wagon::ever_built() == 2);

        // Przypisanie kopiujące to przebudowa – wagon zachowuje swój numer.
        Wagon w4{WagonType::SLEEPER};
        assert(w4.number() == 3);
        w4 = w1;
        assert(w4.number() == 3);
        assert(w4.series() == STANDARD_FIRST_CLASS);

        // Przypisanie kopiujące do pustego obiektu buduje nowy wagon.
        w2 = w1;
        assert(w2.number() == 4);
        assert(Wagon::in_service() == 4);

        // Przypisanie przenoszące – wagon nr 3 przestaje istnieć.
        w4 = std::move(w3);
        assert(w4.number() == 2);
        assert(!w3);
        assert(Wagon::in_service() == 3);

        cout << w1 << '\n' << w4 << '\n' << w3 << "\n\n";
    }

    void temporary_examples() {
        // Wszystkie wagony z poprzedniej funkcji już nie istnieją.
        assert(Wagon::in_service() == 0);

        // Obiekt tymczasowy istnieje do końca pełnego wyrażenia.
        assert(Wagon{STANDARD_DINING}.number() == 5 &&
               Wagon::in_service() == 1);
        assert(Wagon::in_service() == 0);

        {
            // Referencja przedłuża życie obiektu tymczasowego.
            Wagon const& wagon = Wagon{STANDARD_DINING};
            assert(wagon.number() == 6);
            assert(Wagon::in_service() == 1);
        }
        assert(Wagon::in_service() == 0);
    }

    void locomotive_examples() {
        Locomotive eu07{"EU07-005", 3};
        assert(eu07.designation() == "EU07-005");

        // Lokomotyw nie można kopiować, ale można je przenosić.
        Locomotive moved{std::move(eu07)};
        assert(moved);
        assert(!eu07);

        Locomotive none;
        assert(!none);

        cout << moved << '\n' << none << "\n\n";
    }
} // anonimowa przestrzeń nazw

int main() {
    series_examples();
    wagon_examples();
    temporary_examples();
    locomotive_examples();
}
