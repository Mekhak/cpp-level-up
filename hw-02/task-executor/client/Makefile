all: main.o sender.o receiver.o
		g++ main.o sender.o receiver.o -o client

main.o: main.cpp
		g++ -c main.cpp -o main.o

sender.o: sender.cpp
		g++ -c sender.cpp -o sender.o

receiver.o: receiver.cpp
		g++ -c receiver.cpp -o receiver.o

clean:
		rm *.o

