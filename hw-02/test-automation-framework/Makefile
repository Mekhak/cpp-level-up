all: main.o test_executor.o
		g++ main.o test_executor.o -o test_executor

main.o: main.cpp
		g++ -c main.cpp -o main.o

test_executor.o: test_executor.cpp
		g++ -c test_executor.cpp -o test_executor.o

clean:
		rm *.o

