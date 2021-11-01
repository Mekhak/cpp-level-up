all:		main.o message_queue.o server.o
			g++ main.o message_queue.o server.o -o server -lpthread

main.o: 	main.cpp
			g++ -c main.cpp -o main.o -lpthread

message_queue.o: 	message_queue.cpp
					g++ -c message_queue.cpp -o message_queue.o -lpthread

server.o:	server.cpp
			g++ -c server.cpp -o server.o -lpthread

clean:
			rm *.o; rm server

