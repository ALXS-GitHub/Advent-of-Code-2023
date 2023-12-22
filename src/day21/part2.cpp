#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <queue>
#include <set>
#include <unordered_map>
#include <cmath>

using namespace std;
using namespace std::chrono;

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

unordered_map<vector<int>, long long int, VectorHash> cache;

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

long long int get_last_positions(vector<string> input, long long int i, long long int j, long long int remaining)
{
    // ! this will be 100% wrong
    // set<vector<int>> last_positions;

    if (remaining == 0)
    {
        return 1;
    }

    long long int counter = 0;

    int oi = (i + input.size() * 1000) % input.size();
    int oj = (j + input[oi].size() * 1000) % input[oi].size();

    if (oi > 0)
    {
        if (input[oi - 1][oj] != '#') {
            counter += get_last_positions(input, i - 1, j, remaining - 1);
        }
    } else {
        if (input[input.size() - 1][oj] != '#') {
            counter += get_last_positions(input, i - 1, j, remaining - 1);
        }
    }
    if (oi < input.size() - 1)
    {
        if (input[oi + 1][oj] != '#') {
            counter += get_last_positions(input, i + 1, j, remaining - 1);
        }
    } else {
        if (input[0][oj] != '#') {
            counter += get_last_positions(input, i + 1, j, remaining - 1);
        }
    }
    if (oj > 0)
    {
        if (input[oi][oj - 1] != '#') {
            counter += get_last_positions(input, i, j - 1, remaining - 1);
        }
    } else {
        if (input[oi][input[oi].size() - 1] != '#') {
            counter += get_last_positions(input, i, j - 1, remaining - 1);
        }
    }
    if (oj < input[oi].size() - 1)
    {
        if (input[oi][oj + 1] != '#') {
            counter += get_last_positions(input, i, j + 1, remaining - 1);
        }
    } else {
        if (input[oi][0] != '#') {
            counter += get_last_positions(input, i, j + 1, remaining - 1);
        }
    }

    return counter;
}

long long int fill(vector<string> input, long long int is, long long int js, long long int remaining)
{

    set<vector<long long int>> last_positions;
    set<vector<long long int>> new_positions;
    new_positions.insert({is, js});
    for (int k = 0; k < remaining; k++)
    {
        set<vector<long long int>> next_positions;
        for (auto position : new_positions)
        {
            long long int i = position[0];
            long long int j = position[1];
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

    return new_positions.size();
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day21/input.txt");
    vector<int> starting_point = get_starting_point(input);
    long long int is = starting_point[0];
    long long int js = starting_point[1];


    // ! inside
    long long int init_steps = 26501365;
    long long int grid_len = init_steps / input.size() - 1;
    long long int grid_odd = pow((floor(grid_len / 2) * 2 + 1),2);
    cout << "Grid odd: " << grid_odd << endl;
    long long int grid_even = pow((floor((grid_len + 1) / 2) * 2),2);
    cout << "Grid even: " << grid_even << endl;

    
    long long int odd_result = fill(input, is, js, input.size() * 2 + 1);
    long long int even_result = fill(input, is, js, input.size() * 2);

    cout << "Odd result: " << odd_result << endl;
    cout << "Even result: " << even_result << endl;

    // ! corners
    long long int top = fill(input, input.size() - 1, js, input.size() - 1);
    long long int bottom = fill(input, 0, js, input.size() - 1);
    long long int left = fill(input, is, input[0].size() - 1, input.size() - 1);
    long long int right = fill(input, is, 0, input.size() - 1);
    cout << "Corners";

    // ! edges
    long long int ne_out = fill(input, input.size() - 1, 0, floor(input.size() / 2) - 1);
    long long int nw_out = fill(input, input.size() - 1, input[0].size() - 1, floor(input.size() / 2) - 1);
    long long int se_out = fill(input, 0, 0, floor(input.size() / 2) - 1);
    long long int sw_out = fill(input, 0, input[0].size() - 1, floor(input.size() / 2) - 1);
    cout << "Edges Out";

    long long int ne_in = fill(input, input.size() - 1, 0, floor((input.size() * 3) / 2) - 1);
    long long int nw_in = fill(input, input.size() - 1, input[0].size() - 1, floor((input.size() * 3) / 2) - 1);
    long long int se_in = fill(input, 0, 0, floor((input.size() * 3) / 2) - 1);
    long long int sw_in = fill(input, 0, input[0].size() - 1, floor((input.size() * 3) / 2) - 1); 
    cout << "Edges In";

    // ! result
    long long int final_result = (odd_result * grid_odd) + (even_result * grid_even) + top + bottom + left + right;
    final_result += (ne_out + nw_out + se_out + sw_out) * (grid_len + 1);
    final_result += (ne_in + nw_in + se_in + sw_in) * (grid_len);

    cout << "Final result: " << final_result << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}