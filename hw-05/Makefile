all:			main.o listener.o shared_object.o
				g++ main.o listener.o shared_object.o -o shared_object -lpthread

shared_object.o:	shared_object.cpp
				    g++ -c shared_object.cpp -o shared_object.o -lpthread

listener.o:		listener.cpp
				g++ -c listener.cpp -o listener.o -lpthread

main.o:			main.cpp
				g++ -c main.cpp -o main.o -lpthread

clean:
				rm *.o

