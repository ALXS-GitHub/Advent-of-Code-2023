CC = g++

CFLAGS = -Wall -g -Wextra -pedantic -std=c++17

DAYS = day1

all: timer day1

timer:
	$(CC) $(CFLAGS) src/timer.cpp -o bin/timer.exe

$(DAYS): %: %/part1 %/part2

%/part1:
	$(CC) $(CFLAGS) src/$*/part1.cpp -o bin/$*/part1.exe

%/part2:
	$(CC) $(CFLAGS) src/$*/part2.cpp -o bin/$*/part2.exe

clean: 
ifeq ($(OS),Windows_NT)
	del /S bin\*.exe bin\*.o
else 
	find ./bin -type f \( -name "*.exe" -o -name "*.o" \) -exec rm -f {} \;
endif