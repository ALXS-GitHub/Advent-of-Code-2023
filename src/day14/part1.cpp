#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

void roll_rock(vector<string> &input, int i, int j) {
    if (i <= 0 || i >= input.size() || j < 0 || j >= input[i].size()) { // check condition i <= 0
        return;
    }
    if (input[i][j] != 'O') {
        return;
    }

    int new_i = i;

    for (int k = i-1; k >= 0; k--) {
        if (input[k][j] == '.') {
            new_i = k;
        } else {
            break;
        }
    }

    if (new_i != i) {
        input[new_i][j] = 'O';
        input[i][j] = '.';
    }

}

void pull_lever(vector<string> &input) {
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == 'O') {
                roll_rock(input, i, j);
            }
        }
    }
}

long long int total_load(vector<string> &input) {
    long long int total = 0;
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            if (input[i][j] == 'O') {
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
    pull_lever(input);

    cout << total_load(input) << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}