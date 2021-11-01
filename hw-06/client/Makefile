all:		main.o client.o
			g++ main.o client.o -o client -lpthread

main.o: 	main.cpp
			g++ -c main.cpp -o main.o -lpthread

client.o:	client.cpp
			g++ -c client.cpp -o client.o -lpthread

clean:
			rm *.o; rm client 

