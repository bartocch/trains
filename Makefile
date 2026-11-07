CXXFLAGS = -Wall -Wextra -O2 -std=c++23

.PHONY: clean

clean:
	rm -f trains_example

trains_example: trains_example.cpp trains.h
	g++ $(CXXFLAGS) trains_example.cpp -o trains_example

unit_tests: unit_tests.cpp trains.h
	g++ $(CXXFLAGS) unit_tests.cpp -o unit_tests
