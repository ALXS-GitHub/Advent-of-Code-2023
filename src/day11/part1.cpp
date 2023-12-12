#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

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

typedef struct galaxy_t
{
    int n;
    int i;
    int j;
    int voisin = -1;
    int distance = -1;
} galaxy_t;

typedef struct pair_t
{
    galaxy_t galaxy1;
    galaxy_t galaxy2;
} pair_t;

unordered_map<int, galaxy_t> galaxies;
unordered_map<vector<int>, pair_t, VectorHash> known_pairs;

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
            for (int i = 0; i < input.size(); i++)
            {
                input[i].insert(j, ".");
            }
            j++;
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
            input.insert(input.begin() + i, string(input[0].size(), '.'));
            i++;
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
            galaxies[counter] = galaxy;
            counter++;
        }
    }
}

galaxy_t get_galaxy_from_i_j(vector<string> &input, int i, int j)
{
    for (auto [k,v] : galaxies)
    {
        if (v.i == i && v.j == j)
        {
            return v;
        }
    }
    return {-1, -1, -1};
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

int get_shortest_path(vector<string> &input, galaxy_t &galaxy)
{
    int sum = 0;

    for (auto [k,v] : galaxies)
    {
        if (galaxy.n == v.n)
        {
            continue;
        }
        if (is_known_pair(galaxy, v))
        {
            continue;
        }
        sum += abs(galaxy.i - v.i) + abs(galaxy.j - v.j);
        known_pairs[{galaxy.n, v.n}] = {galaxy, v};
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

    int sum = 0;

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