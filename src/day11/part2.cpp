#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

#define SPACE_SIZE 1000000

using namespace std;
using namespace std::chrono;

typedef struct galaxy_t
{
    int n;
    int i;
    int j;
} galaxy_t;

struct VectorHash
{
    size_t operator()(const std::vector<int> &v) const
    {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v)
        {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

typedef struct visited_tile_t
{
    int i;
    int j;
    long long int distance;
} visited_tile_t;

typedef struct visited_t
{
    unordered_map<vector<int>, visited_tile_t, VectorHash> visited;
    vector<visited_tile_t> new_visited;
} visited_t;

typedef struct pair_t
{
    galaxy_t galaxy1;
    galaxy_t galaxy2;
} pair_t;

unordered_map<vector<int>, galaxy_t, VectorHash> galaxies;
unordered_map<vector<int>, pair_t, VectorHash> known_pairs;
unordered_map<int, int> empty_rows;
unordered_map<int, int> empty_cols;

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

bool is_col_empty(vector<string> &input, int col)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i][col] != '.')
        {
            return false;
        }
    }
    return true;
}

bool is_row_empty(string &row)
{
    for (int i = 0; i < row.size(); i++)
    {
        if (row[i] != '.')
        {
            return false;
        }
    }
    return true;
}

void expend_cols(vector<string> &input)
{
    int j = 0;
    while (j < input[0].size())
    {
        if (is_col_empty(input, j))
        {
            empty_cols[{j}] = 1;
        } else {
            empty_cols[{j}] = 0;
        }
        j++;
    }
}

void expend_rows(vector<string> &input)
{
    int i = 0;
    while (i < input.size())
    {
        if (is_row_empty(input[i]))
        {
            empty_rows[{i}] = 1;
        } else {
            empty_rows[{i}] = 0;
        }
        i++;
    }
}

void expend_universe(vector<string> &input)
{
    expend_cols(input);
    expend_rows(input);
}

void get_galaxies(vector<string> &input)
{

    int counter = 0;

    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (input[i][j] == '.')
            {
                continue;
            }
            galaxy_t galaxy;
            galaxy.n = counter;
            galaxy.i = i;
            galaxy.j = j;
            galaxies[{i, j}] = galaxy;
            counter++;
        }
    }
}

bool is_known_pair(galaxy_t &galaxy1, galaxy_t &galaxy2)
{
    if (known_pairs.find({galaxy1.n, galaxy2.n}) != known_pairs.end())
    {
        return true;
    }
    if (known_pairs.find({galaxy2.n, galaxy1.n}) != known_pairs.end())
    {
        return true;
    }
    return false;
}

long long int get_shortest_path(vector<string> &input, galaxy_t &galaxy)
{
    long long int sum = 0;

    for (auto [k, v] : galaxies)
    {
        if (galaxy.n == v.n)
        {
            continue;
        }
        if (is_known_pair(galaxy, v))
        {
            continue;
        }
        known_pairs[{galaxy.n, v.n}] = {galaxy, v};

        int max_i = max(galaxy.i, v.i);
        int min_i = min(galaxy.i, v.i);

        for (int i = min_i; i < max_i; i++)
        {
            if (empty_rows[{i}] == 1)
            {
                sum += SPACE_SIZE;
            }
            else
            {
                sum += 1;
            }
        }

        int max_j = max(galaxy.j, v.j);
        int min_j = min(galaxy.j, v.j);

        for (int j = min_j; j < max_j; j++)
        {
            if (empty_cols[{j}] == 1)
            {
                sum += SPACE_SIZE;
            }
            else
            {
                sum += 1;
            }
        }
    }

    return sum;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day11/input.txt");
    expend_universe(input);

    get_galaxies(input);

    long long int sum = 0;

    for (auto it = galaxies.begin(); it != galaxies.end(); it++)
    {
        sum += get_shortest_path(input, it->second);
    }

    cout << sum << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}