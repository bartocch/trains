CXXFLAGS = -Wall -Wextra -O2 -std=c++23

.PHONY: clean

clean:
	rm -f trains_example examples/trains_example examples/trains_example_part_1

examples/trains_example: examples/trains_example.cpp trains.h
	g++ $(CXXFLAGS) examples/trains_example.cpp -o examples/trains_example

examples/trains_example_part_1: examples/trains_example_part_1.cpp trains.h
	g++ $(CXXFLAGS) examples/trains_example_part_1.cpp -o examples/trains_example_part_1

unit_tests: unit_tests.cpp trains.h
	g++ $(CXXFLAGS) unit_tests.cpp -o unit_tests
