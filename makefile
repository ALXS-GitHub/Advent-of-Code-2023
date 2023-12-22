CC = g++

CFLAGS = -Wall -g -Wextra -pedantic -std=c++17

DAYS = day1 day2 day3 day4 day5 day6 day7 day8 day9 day10 day11 day12 day13 day14 day15 day16 day17 day18 day19 day20 day21 day22 day23 day24 day25

all: timer $(DAYS)

timer: bin
	$(CC) $(CFLAGS) src/timer.cpp -o bin/timer.exe

$(DAYS): %: %-bin %/part1 %/part2

newday: 
	$(CC) $(CFLAGS) newday.cpp -o newday.exe

bin: 
ifeq ($(OS),Windows_NT)
	if not exist bin mkdir bin
else 
	mkdir -p bin
endif

%-bin: bin
ifeq ($(OS),Windows_NT)
	if not exist bin\$* mkdir bin\$*
else 
	mkdir -p bin/$*
endif

%/part1: %-bin
	$(CC) $(CFLAGS) src/$*/part1.cpp -o bin/$*/part1.exe

%/part2: %-bin
	$(CC) $(CFLAGS) src/$*/part2.cpp -o bin/$*/part2.exe

clean: 
ifeq ($(OS),Windows_NT)
	del /S bin\*.exe bin\*.o
else 
	find ./bin -type f \( -name "*.exe" -o -name "*.o" \) -exec rm -f {} \;
endif

clean-debug:
ifeq ($(OS),Windows_NT)
	del /S debug\*.exe debug\*.o
else 
	find ./debug -type f \( -name "*.exe" -o -name "*.o" \) -exec rm -f {} \;
endif