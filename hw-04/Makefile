all:			main.o thread_pool.o
				g++ main.o thread_pool.o -o thread_pool -lpthread

thread_pool.o:	thread_pool.cpp
				g++ -c thread_pool.cpp -o thread_pool.o -lpthread

main.o:			main.cpp
				g++ -c main.cpp -o main.o -lpthread

clean:
				rm *.o

