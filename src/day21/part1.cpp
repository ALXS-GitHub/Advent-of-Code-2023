#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <queue>
#include <set>

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

vector<int> get_starting_point(vector<string> input)
{
    vector<int> starting_point;
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (input[i][j] == 'S')
            {
                starting_point.push_back(i);
                starting_point.push_back(j);
                return starting_point;
            }
        }
    }
    return starting_point;
}

set<vector<int>> get_last_positions(vector<string> input, int is, int js) {
    set<vector<int>> last_positions;
    set<vector<int>> new_positions;
    new_positions.insert({is, js});
    for (int i = 0; i < 64; i++) {
        set<vector<int>> next_positions;
        for (auto position : new_positions) {
            int i = position[0];
            int j = position[1];
            if (i > 0 && input[i - 1][j] != '#')
            {
                next_positions.insert({i - 1, j});
            }
            if (i < input.size() - 1 && input[i + 1][j] != '#')
            {
                next_positions.insert({i + 1, j});
            }
            if (j > 0 && input[i][j - 1] != '#')
            {
                next_positions.insert({i, j - 1});
            }
            if (j < input[i].size() - 1 && input[i][j + 1] != '#')
            {
                next_positions.insert({i, j + 1});
            }
        }
        last_positions.insert(new_positions.begin(), new_positions.end());
        new_positions = next_positions;
        next_positions.clear();
    }

    if (new_positions.size() > 0) {
        last_positions.insert(new_positions.begin(), new_positions.end());
    }

    return new_positions;
}


int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer





    vector<string> input = read_input("./inputs/day21/input.txt");
    vector<int> starting_point = get_starting_point(input);
    int is = starting_point[0];
    int js = starting_point[1];

    set<vector<int>> last_positions = get_last_positions(input, is, js);

    cout << "Last positions: " << last_positions.size() << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}