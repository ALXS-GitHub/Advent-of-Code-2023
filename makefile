CC = g++

CFLAGS = -Wall -g -Wextra -pedantic -std=c++17

DAYS = day1 day2 day3 day4 day5 day6 day7

all: timer $(DAYS)

timer: bin
	$(CC) $(CFLAGS) src/timer.cpp -o bin/timer.exe

$(DAYS): %: %-bin %/part1 %/part2

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