#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

typedef struct potition_t
{
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

bool is_A_end (string actual) {
    if (actual[2] == 'A') {
        return true;
    }
    return false;
}

bool is_Z_end (string actual) {
    if (actual[2] == 'Z') {
        return true;
    }
    return false;
}

bool is_all_Z_end (vector<position_t> all_Z_end) {
    for (int i = 0; i < all_Z_end.size(); i++) {
        if (!is_Z_end(all_Z_end[i].actual)) {
            return false;
        }
    }
    return true;
}

vector<position_t> get_all_A_end() {
    vector<position_t> all_A_end;
    for (auto it = registers.begin(); it != registers.end(); it++) {
        if (is_A_end(it->first)) {
            all_A_end.push_back(it->second);
        }
    }
    return all_A_end;
}

void get_maps(vector<string> input)
{
    for (int i = 2; i < input.size(); i++)
    {
        string line = input[i];
        string actual = line.substr(0, 3);
        string left = line.substr(7, 3);
        string right = line.substr(12, 3);
        position_t position = {actual, left, right};
        registers[actual] = position;
    }
}

unsigned long long int follow_instructions(string instruction, vector<position_t> all_A_end)
{
    int i = 0;
    unsigned long long int counter = 0;
    vector<position_t> positions = all_A_end;
    while (!is_all_Z_end(positions))
    {
        if (i >= instruction.size())
        {
            i = 0;
            continue;
        }
        else if (instruction[i] == 'L')
        {
            for (int j = 0; j < positions.size(); j++)
            {
                positions[j].actual = registers[positions[j].actual].left;
            }
            counter++;
        }
        else if (instruction[i] == 'R')
        {
            for (int j = 0; j < positions.size(); j++)
            {
                positions[j].actual = registers[positions[j].actual].right;
            }
            counter++;
        }
        else
        {
            cerr << "Error" << endl;
        }
        i++;
    }
    return counter;
}

unsigned long long int get_first_Z_end(string instruction, position_t actual) {
    unsigned long long int counter = 0;
    int i = 0;
    while (!is_Z_end(actual.actual)) {
        if (i >= instruction.size())
        {
            i = 0;
            continue;
        }
        else if (instruction[i] == 'L')
        {
            actual.actual = registers[actual.actual].left;
            counter++;
        }
        else if (instruction[i] == 'R')
        {
            actual.actual = registers[actual.actual].right;
            counter++;
        }
        else
        {
            cerr << "Error" << endl;
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
    cout << instruction.size();
    get_maps(input);
    vector<position_t> all_A_end = get_all_A_end();

    for (auto it = all_A_end.begin(); it != all_A_end.end(); it++)
    {
        cout << it->actual << " " << it->left << " " << it->right << endl;
    }

    vector<unsigned long long int> counters;

    for (int i = 0; i < all_A_end.size(); i++) {
        unsigned long long int c = get_first_Z_end(instruction, all_A_end[i]);
        counters.push_back(c);
    }

    // for (int i = 0; i < counters.size(); i++) {
    //     cout << counters[i] << endl;
    // }

    // get the ppcm of all counters
    unsigned long long int ppcm = counters[0];
    for (int i = 1; i < counters.size(); i++) {
        unsigned long long int a = ppcm;
        unsigned long long int b = counters[i];
        while (a != b) {
            if (a < b) {
                a += ppcm;
            } else {
                b += counters[i];
            }
        }
        ppcm = a;
    }

    cout << "PPCM: " << ppcm << endl;

    // ! timer
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}