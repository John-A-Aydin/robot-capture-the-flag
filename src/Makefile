sim: main.o sim.o
	g++ -std=c++20 main.o sim.o -o sim -lSDL2 -lSDL2_ttf

main.o: main.cpp
	g++ -c -std=c++20 main.cpp -lSDL2 -lSDL2_ttf

sim.o: sim.cpp sim.h
	g++ -c -std=c++20 sim.cpp -lSDL2 -lSDL2_ttf

clean:
	rm *.o sim