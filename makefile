CC = g++

CFLAGS = -Wall -g -Wextra -pedantic -std=c++17

all: timer day1

timer:
	$(CC) $(CFLAGS) src/timer.cpp -o bin/timer.exe

day1: day1/part1 day1/part2

day1/part1:
	$(CC) $(CFLAGS) src/day1/part1.cpp -o bin/day1/part1.exe

day1/part2:
	$(CC) $(CFLAGS) src/day1/part2.cpp -o bin/day1/part2.exe

clean: 
	rm -f /bin/*.o /bin/*.exe