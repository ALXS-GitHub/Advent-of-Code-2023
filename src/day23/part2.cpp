#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <map>
#include <queue>
#include <deque>
#include <set>
#include <cassert>
#include <climits>

using namespace std;
using namespace std::chrono;

typedef struct tile_t
{
    int i;
    int j;
    long long int distance;
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

class Compare
{
public:
    bool operator()(tile_t a, tile_t b)
    {
        return a.distance > b.distance;
    }
};

unordered_map<vector<int>, map<vector<int>, int>, VectorHash> graph;
unordered_map<vector<int>, int, VectorHash> paths;

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

vector<vector<int>> travel(vector<string> input) {
    int si = input.size();
    int sj = input[0].size();
    vector<int> start = {0, 1};
    vector<int> end = {si - 1, sj - 2};

    vector<vector<int>> points;
    points.push_back(start);
    points.push_back(end);

    for (int i = 0; i < si; i++) {
        for (int j = 0; j < sj; j++) {
            if (input[i][j] == '#') {
                continue;
            }
            int neighbors = 0;
            vector<vector<int>> next = {{i - 1, j}, {i + 1, j}, {i, j - 1}, {i, j + 1}};  
            for (auto n : next) {
                if (n[0] < 0 || n[0] >= si || n[1] < 0 || n[1] >= sj) {
                    continue;
                }
                if (input[n[0]][n[1]] == '#') {
                    continue;
                }
                neighbors++;
            }
            if (neighbors >= 3) {
                points.push_back({i, j});
            }
        }
    }
    return points;
}

bool is_in_vector(vector<vector<int>> v, vector<int> p) {
    for (auto i : v) {
        if (i[0] == p[0] && i[1] == p[1]) {
            return true;
        }
    }
    return false;
}

void setup_graph(vector<string> input, vector<vector<int>> points) {

    for (auto p : points) {
        vector<vector<int>> stack;
        vector<int> pt = {p[0], p[1], 0};
        stack.push_back(pt);
        set<vector<int>> seen;
        seen.insert(p);

        while(stack.size() > 0) {
            vector<int> current = stack.back();
            stack.pop_back();
            
            if (current[2] != 0 && is_in_vector(points, {current[0], current[1]})) {
                graph[{p[0], p[1]}][{current[0], current[1]}] = current[2];
                continue;
            }

            vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            for (auto d : directions)
            {
                int nr = current[0] + d[0];
                int nc = current[1] + d[1];
                if (nr < 0 || nr >= input.size() || nc < 0 || nc >= input[0].size()) {
                    continue;
                }
                if (input[nr][nc] == '#') {
                    continue;
                }
                if (seen.find({nr, nc}) != seen.end()) {
                    continue;
                }
                stack.push_back({nr, nc, current[2] + 1});
                seen.insert({nr, nc});
            }
        }
    }

}

set<vector<int>> seen_dfs;

int dfs(vector<int> point, vector<int> end) {
    if (point[0] == end[0] && point[1] == end[1]) {
        return 0;
    }

    int max_dist = INT_MIN;

    seen_dfs.insert(point);
    for (auto n : graph[point]) {
        if (seen_dfs.find(n.first) != seen_dfs.end()) {
            continue;
        }
        max_dist = max(max_dist, dfs(n.first, end) + n.second);
    }
    seen_dfs.erase(point);

    return max_dist;

}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day23/input.txt");
    int si = input.size();
    int sj = input[0].size();
    vector<int> start = {0, 1};
    vector<int> end = {si - 1, sj - 2};

    vector<vector<int>> points = travel(input);
    setup_graph(input, points);

    // for (auto g : graph) {
    //     cout << g.first[0] << "," << g.first[1] << " -> ";
    //     for (auto n : g.second) {
    //         cout << n.first[0] << "," << n.first[1] << " : " << n.second << " |";
    //     }
    //     cout << endl;
    // }

    cout << dfs(start, end) << endl;

    

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}