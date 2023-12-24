#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <queue>
#include <deque>

using namespace std;
using namespace std::chrono;

typedef struct tile_t {
    int i;
    int j;
    long long int distance;
    // int prev_i;
    // int prev_j;
} tile_t;

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

class Compare {
    public:
        bool operator() (tile_t a, tile_t b) {
            return a.distance > b.distance;
        }
};

long long int get_longest_path(vector<string> input) {
    int si = size(input);
    int sj = size(input[0]);
    vector<int> start = {0, 1};
    vector<int> end = {si - 1, sj - 2};
    long long int longest_path = 0;

    unordered_map<vector<int>, long long int, VectorHash> seen;
    priority_queue<tile_t, vector<tile_t>, Compare> q;

    tile_t start_tile = {start[0], start[1], 0};
    q.push(start_tile);

    while (!q.empty()) {
        // cout << "Queue size: " << q.size() << endl;
        tile_t current = q.top();
        q.pop();

        if (current.i == end[0] && current.j == end[1]) {
            if (current.distance > longest_path) {
                longest_path = current.distance;
            }
            continue;
        }

        vector<int> current_pos = {current.i, current.j};
        if (seen.find(current_pos) != seen.end()) {
            // cout << "Seen " << current.i << ", " << current.j << " before" << endl;
            if (seen[current_pos] + 2 < current.distance) {
                seen[current_pos] = current.distance;
            }
            else if (seen[current_pos] <= current.distance) {
                seen[current_pos] = current.distance;
                continue;
            }
            
        } else {
            seen[current_pos] = current.distance;
        }

        if (current.i > 0 && input[current.i - 1][current.j] == '.') {
            tile_t next = {current.i - 1, current.j, current.distance + 1};
            q.push(next);
        }
        if (current.i < si - 1 && input[current.i + 1][current.j] == '.') {
            tile_t next = {current.i + 1, current.j, current.distance + 1};
            q.push(next);
        }
        if (current.j > 0 && input[current.i][current.j - 1] == '.') {
            tile_t next = {current.i, current.j - 1, current.distance + 1};
            q.push(next);
        }
        if (current.j < sj - 1 && input[current.i][current.j + 1] == '.') {
            tile_t next = {current.i, current.j + 1, current.distance + 1};
            q.push(next);
        }
        if (current.j < sj && input[current.i][current.j + 1] == '>')
        {
            tile_t next = {current.i, current.j + 1, current.distance + 1};
            q.push(next);
        }
        if (current.i < si && input[current.i + 1][current.j] == 'v')
        {
            tile_t next = {current.i + 1, current.j, current.distance + 1};
            q.push(next);
        }
    }

    return longest_path;

}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer





    vector<string> input = read_input("./inputs/day23/input.txt");
    long long int longest_path = get_longest_path(input);
    cout << "Longest path: " << longest_path << endl;
    


    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}