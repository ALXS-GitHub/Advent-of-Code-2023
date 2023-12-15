#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

#define CYCLES 1000000000

using namespace std;
using namespace std::chrono;

struct VectorHash {
    size_t operator()(const vector<string>& v) const {
        hash<string> hasher;
        size_t seed = 0;
        for (string s : v) {
            seed ^= hasher(s) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};


unordered_map<vector<string>, long long int, VectorHash> cache;

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

void roll_north(vector<string> &input, int i, int j)
{
    if (i <= 0 || i >= input.size() || j < 0 || j >= input[i].size())
    { // check condition i <= 0
        return;
    }
    if (input[i][j] != 'O')
    {
        return;
    }

    int new_i = i;

    for (int k = i - 1; k >= 0; k--)
    {
        if (input[k][j] == '.')
        {
            new_i = k;
        }
        else
        {
            break;
        }
    }

    if (new_i != i)
    {
        input[new_i][j] = 'O';
        input[i][j] = '.';
    }
}

void pull_north(vector<string> &input)
{
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (input[i][j] == 'O')
            {
                roll_north(input, i, j);
            }
        }
    }
}

void roll_west(vector<string> &input, int i, int j)
{
    if (i < 0 || i >= input.size() || j <= 0 || j >= input[i].size())
    { // check condition j <= 0
        return;
    }
    if (input[i][j] != 'O')
    {
        return;
    }

    int new_j = j;

    for (int k = j - 1; k >= 0; k--)
    {
        if (input[i][k] == '.')
        {
            new_j = k;
        }
        else
        {
            break;
        }
    }

    if (new_j != j)
    {
        input[i][new_j] = 'O';
        input[i][j] = '.';
    }
}

void pull_west(vector<string>& input) {
    for (int j = 0; j < input[0].size(); j++) {
        for (int i = 0; i < input.size() ; i++) {
            if (input[i][j] == 'O') {
                roll_west(input, i, j);
            }
        }
    }

}

void roll_south(vector<string> &input, int i, int j)
{
    if (i < 0 || i >= input.size() - 1 || j < 0 || j >= input[i].size())
    { // check condition i >= input.size() - 1
        return;
    }
    if (input[i][j] != 'O')
    {
        return;
    }

    int new_i = i;

    for (int k = i + 1; k < input.size(); k++)
    {
        if (input[k][j] == '.')
        {
            new_i = k;
        }
        else
        {
            break;
        }
    }

    if (new_i != i)
    {
        input[new_i][j] = 'O';
        input[i][j] = '.';
    }
}

void pull_south(vector<string>& input) {
    for (int i = input.size() - 1; i >= 0; i--) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == 'O') {
                roll_south(input, i, j);
            }
        }
    }
}

void roll_east(vector<string> &input, int i, int j)
{
    if (i < 0 || i >= input.size() || j < 0 || j >= input[i].size() - 1)
    { // check condition j >= input[i].size() - 1
        return;
    }
    if (input[i][j] != 'O')
    {
        return;
    }

    int new_j = j;

    for (int k = j + 1; k < input[i].size(); k++)
    {
        if (input[i][k] == '.')
        {
            new_j = k;
        }
        else
        {
            break;
        }
    }

    if (new_j != j)
    {
        input[i][new_j] = 'O';
        input[i][j] = '.';
    }
}

void pull_east(vector<string>& input) {
    for (int j = input[0].size() - 1; j >= 0; j--) {
        for (int i = 0; i < input.size(); i++) {
            if (input[i][j] == 'O') {
                roll_east(input, i, j);
            }
        }
    }
}

void cycle(vector<string>& input) {

    pull_north(input);
    pull_west(input);
    pull_south(input);
    pull_east(input);

}

long long int total_load(vector<string> &input)
{
    long long int total = 0;
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (input[i][j] == 'O')
            {
                total += input.size() - i;
            }
        }
    }
    return total;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day14/input.txt");
    vector<string> default_input = input;
    cache[input] = 0;
    long long int cycle_start = 0;
    long long int cycle_count = 0;

    for (int i = 0; i < CYCLES; i++) {
        cycle(input);
        cycle_count++;
        if (cache.find(input) != cache.end()) {
            cycle_start = cache[input];
            break;
        }
        cache[input] = cycle_count;
    }

    long long int cycle_length = cycle_count - cycle_start;
    long long int cycle_remainder = (CYCLES - cycle_start) % cycle_length;

    for (int i = 0; i < cycle_remainder; i++) {
        cycle(input);
    }

    cout << total_load(input) << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}