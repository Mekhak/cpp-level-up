all: main.o task_manager.o
		g++ main.o task_manager.o -o task-manager

main.o: main.cpp
		g++ -c main.cpp -o main.o

task_manager.o: task_manager.cpp
		g++ -c task_manager.cpp -o task_manager.o

clean:
		rm *.o

