all:		main.o message_queue.o
			g++ main.o message_queue.o -lpthread -o message_queue

main.o:		main.cpp
			g++ -c main.cpp -lpthread -o main.o

message_queue.o:	message_queue.cpp
					g++ -c message_queue.cpp -lpthread -o message_queue.o

clean:
			rm -r *.o

