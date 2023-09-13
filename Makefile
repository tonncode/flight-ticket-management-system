output: main.o flighthashtable.o flightticket.o
	g++ main.o flighthashtable.o flightticket.o -o output

main.o: main.cpp
	g++ -c main.cpp

flighthashtable.o: flighthashtable.cpp
	g++ -c flighthashtable.cpp
	
flightticket.o: flightticket.cpp
	g++ -c flightticket.cpp

clean:
	rn *.o output
