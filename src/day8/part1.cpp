#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

typedef struct potition_t {
    string actual;
    string left;
    string right;
} position_t;

unordered_map<string, position_t> registers;

vector<string> read_input(string filename)
{
    vector<string> input;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line.back() == '\r' && line.size() > 0)
            {
                line.pop_back();
            }
            input.push_back(line);
        }
        cout << "Read " << input.size() << " lines from " << filename << endl;
        file.close();
    }
    return input;
}

void get_maps(vector<string> input) {
    for (int i = 2; i < input.size(); i++) {
        string line = input[i];
        string actual = line.substr(0, 3);
        string left = line.substr(7, 3);
        string right = line.substr(12, 3);
        position_t position = {actual, left, right};
        registers[actual] = position;
    }
}

unsigned long long int follow_instructions(string instruction, string actual, string last) {
    int i = 0;
    unsigned long long int counter = 0;
    position_t position = registers[actual];
    while (position.actual != "ZZZ") {
        if (i >= instruction.size()) {
            i = 0;
            continue;
        }
        else if (instruction[i] == 'L') {
            position = registers[position.left];
            counter++;
        } else if (instruction[i] == 'R') {
            position = registers[position.right];
            counter++;
        } else {
            cerr << "Error" << endl;
        }
        if (counter % 100000000 == 0) {
            cout << instruction[i] << endl;
            cout << position.actual << " " << position.left << " " << position.right << endl;
            cout << counter << endl;
        }
        i++;
    }
    return counter;
}

int main()
{
    // ! timer
    auto start = high_resolution_clock::now();
    // ! timer





    vector<string> input = read_input("./inputs/day8/input.txt");
    string instruction = input[0];
    string actual = "AAA";
    string last = input[input.size() - 1].substr(0, 3);
    get_maps(input);

    unsigned long long int counter = follow_instructions(instruction, actual, last);

    cout << "Counter: " << counter << endl;





    // ! timer
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}