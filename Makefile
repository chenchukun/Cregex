main: main.o DFA.o NFA.o
	g++ main.o DFA.o NFA.o -o main -std=c++11
main.o: main.cpp
	g++ -c main.cpp -std=c++11
NFA.o: NFA.cpp
	g++ -c NFA.cpp -std=c++11
DFA.o: DFA.cpp
	g++ -c DFA.cpp -std=c++11
.PHONY: clean
clean:
	rm *.o
