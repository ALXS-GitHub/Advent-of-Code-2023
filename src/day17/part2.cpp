#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <queue>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

//    0
//  3   1
//    2

typedef struct crucible_t
{
    int i;
    int j;
    int direction;
    int consecutive = 0;
    long long int distance = 0;
} crucible_t;

struct CrucibleHash
{
    size_t operator()(const crucible_t &c) const
    {
        std::hash<int> hasher;
        size_t seed = 0;
        seed ^= hasher(c.i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(c.j) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(c.direction) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(c.consecutive) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct CrucibleEqual
{
    bool operator()(const crucible_t &lhs, const crucible_t &rhs) const
    {
        return lhs.i == rhs.i && lhs.j == rhs.j && lhs.direction == rhs.direction && lhs.consecutive == rhs.consecutive;
    }
};

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

typedef struct path_t
{
    vector<crucible_t> crucibles;
} path_t;

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

void vector_remove(vector<crucible_t> &v, crucible_t crucible)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].i == crucible.i && v[i].j == crucible.j && v[i].direction == crucible.direction)
        {
            v.erase(v.begin() + i);
            return;
        }
    }
    return;
}

crucible_t crucible_step(vector<string> input, crucible_t crucible)
{
    int i = crucible.i;
    int j = crucible.j;
    int d = crucible.direction;
    int si = input.size();
    int sj = input[0].size();

    if (d == 0)
    {
        i -= 1;
        crucible.i = i;
    }
    else if (d == 1)
    {
        j += 1;
        crucible.j = j;
    }
    else if (d == 2)
    {
        i += 1;
        crucible.i = i;
    }
    else if (d == 3)
    {
        j -= 1;
        crucible.j = j;
    }

    if (i < 0 || i >= si || j < 0 || j >= sj)
    {
        return {-1, -1, -1, -1, -1};
    }
    else
    {
        return crucible;
    }
}

bool vector_contains(vector<crucible_t> v, crucible_t crucible)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].i == crucible.i && v[i].j == crucible.j && v[i].direction == crucible.direction)
        {
            return true;
        }
    }
    return false;
}

crucible_t &get_from_vector(vector<crucible_t> &v, crucible_t crucible)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].i == crucible.i && v[i].j == crucible.j && v[i].direction == crucible.direction)
        {
            return v[i];
        }
    }
}

crucible_t get_from_vector_coord(vector<crucible_t> v, crucible_t crucible)
{
    long long int min = 1000000000;
    crucible_t best_crucible = {-1, -1, -1, -1, -1};
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].i == crucible.i && v[i].j == crucible.j)
        {
            if (v[i].distance < min)
            {
                min = v[i].distance;
                best_crucible = v[i];
            }
        }
    }
    return best_crucible;
}

vector<crucible_t> valid_destination(vector<string> input, crucible_t crucible)
{
    int d1 = crucible.direction;
    int d2 = (crucible.direction + 1) % 4;
    int d3 = (crucible.direction - 1 + 4) % 4;

    vector<crucible_t> destinations;
    if (crucible.consecutive < 3)
    {
        crucible_t crucible1 = crucible_step(input, crucible);
        if (crucible1.distance != -1)
        {
            // cout << "1 : " << crucible1.i << " " << crucible1.j << " " << crucible1.direction << " " << crucible1.consecutive << " " << crucible1.distance << endl;
            crucible1.direction = d1;
            crucible1.consecutive = crucible.consecutive + 1;
            destinations.push_back(crucible1);
        }
    }
    crucible.direction = d2;
    crucible_t crucible2 = crucible_step(input, crucible);
    if (crucible2.distance != -1)
    {
        crucible2.direction = d2;
        crucible2.consecutive = 1;
        // cout << "2 : " << crucible2.i << " " << crucible2.j << " " << crucible2.direction << " " << crucible2.consecutive << " " << crucible2.distance << endl;
        destinations.push_back(crucible2);
    }
    crucible.direction = d3;
    crucible_t crucible3 = crucible_step(input, crucible);
    if (crucible3.distance != -1)
    {
        crucible3.direction = d3;
        crucible3.consecutive = 1;
        // cout << "3 : " << crucible3.i << " " << crucible3.j << " " << crucible3.direction << " " << crucible3.consecutive << " " << crucible3.distance << endl;
        destinations.push_back(crucible3);
    }
    crucible.direction = d1;
    return destinations;
}

class Compare
{
public:
    bool operator()(crucible_t &c1, crucible_t &c2)
    {
        return c1.distance >= c2.distance;
    }
};

unordered_map<vector<int>, long long int, VectorHash> get_all(vector<string> input)
{

    priority_queue<crucible_t, vector<crucible_t>, Compare> queue;
    unordered_map<vector<int>, long long int, VectorHash> visited;
    int si = input.size();
    int sj = input[0].size();

    queue.push({0, 0, -1, -1, 0});

    while (!queue.empty())
    {
        // cout << queue.size() << endl;
        crucible_t current_crucible = queue.top();
        queue.pop();

        int i = current_crucible.i;
        int j = current_crucible.j;
        int d = current_crucible.direction;
        int c = current_crucible.consecutive;

        vector<int> v = {i, j, d, c};

        if (visited.find(v) != visited.end())
        {
            continue;
        }
        visited[v] = current_crucible.distance;
        // cout << i << " " << j << " " << d << " " << c << " " << current_crucible.distance << endl;
        vector<vector<int>> directions = {{-1, 0, 0}, {0, 1, 1}, {1, 0, 2}, {0, -1, 3}};
        for (auto v : directions)
        {
            int new_i = current_crucible.i + v[0];
            int new_j = current_crucible.j + v[1];
            int new_d = v[2];
            int new_c = (new_d == current_crucible.direction) ? current_crucible.consecutive + 1 : 1;
            if (new_i < 0 || new_i >= si || new_j < 0 || new_j >= sj || new_c > 10 || ((new_d + 2) % 4 == current_crucible.direction))
            {
                continue;
            }
            if (!(new_d == current_crucible.direction || c >= 4 || c == -1))
            {
                continue;
            }
            int cost = input[new_i][new_j] - '0';
            queue.push({new_i, new_j, new_d, new_c, current_crucible.distance + cost});
        }
    }

    return visited;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day17/input.txt");
    unordered_map<vector<int>, long long int, VectorHash> visited = get_all(input);
    cout << visited.size() << endl;

    int si = input.size();
    int sj = input[0].size();
    long long int min = 1000000000;
    for (auto it = visited.begin(); it != visited.end(); it++)
    {
        vector<int> v = it->first;
        if (v[0] == si - 1 && v[1] == sj - 1)
        {
            if (it->second < min)
            {
                min = it->second;
            }
        }
    }

    cout << min << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}