all: build/robot-game

.PHONY: all

build:
	mkdir -p $@

build/robot-game: build/main.o build/sim.o build/circle.o | build
	g++ -std=c++20 -o $@ $^ -lm -lSDL2 -lSDL2_ttf

build/main.o: src/main.cpp | build
	g++ -std=c++20 -o $@ -c $^

build/sim.o: src/sim.cpp | build
	g++ -std=c++20 -o $@ -c $^

build/circle.o: src/circle.cpp | build
	g++ -std=c++20 -o $@ -c $^



clean:
	rm -rf build

sim: build/sim_full.o build/sim.o | build
	g++ -std=c++20 -o build/$@ $^

build/sim_full.o: src/sim_full.cpp | build
	g++ -std=c++20 -o $@ -c $^ -lm 