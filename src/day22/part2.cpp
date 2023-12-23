#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cassert>
#include <set>
#include <unordered_map>
#include <deque>

using namespace std;
using namespace std::chrono;

typedef struct brick_t
{
    int id;
    int x_start;
    int x_end;
    int y_start;
    int y_end;
    int z_start;
    int z_end;
} brick_t;

vector<vector<vector<int>>> x_grid;
vector<vector<vector<int>>> y_grid;
int max_x = 0;
int max_y = 0;
int max_z = 0;
unordered_map<int, set<int>> supports;
unordered_map<int, set<int>> supported_by;

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

void remove_from_vector(vector<int> &v, int id)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == id)
        {
            v.erase(v.begin() + i);
            return;
        }
    }
}

vector<brick_t> parse_bricks(vector<string> input)
{
    vector<brick_t> bricks;
    for (int i = 0; i < input.size(); i++)
    {
        string line = input[i];
        string start = line.substr(0, line.find("~"));
        string end = line.substr(line.find("~") + 1);
        brick_t brick;
        brick.id = i + 1;
        brick.x_start = stoi(start.substr(0, start.find(",")));
        start = start.substr(start.find(",") + 1);
        brick.y_start = stoi(start.substr(0, start.find(",")));
        start = start.substr(start.find(",") + 1);
        brick.z_start = stoi(start);

        brick.x_end = stoi(end.substr(0, end.find(",")));
        end = end.substr(end.find(",") + 1);
        brick.y_end = stoi(end.substr(0, end.find(",")));
        end = end.substr(end.find(",") + 1);
        brick.z_end = stoi(end);

        if (brick.x_end > max_x)
        {
            max_x = brick.x_end;
        }
        if (brick.y_end > max_y)
        {
            max_y = brick.y_end;
        }
        if (brick.z_end > max_z)
        {
            max_z = brick.z_end;
        }

        bricks.push_back(brick);
    }
    return bricks;
}

vector<brick_t> order_bricks(vector<brick_t> bricks)
{
    vector<brick_t> ordered_bricks;
    int zs = max_z;
    for (int j = 1; j <= zs; j++)
    {
        for (int i = 0; i < bricks.size(); i++)
        {
            if (bricks[i].z_start == j)
            {
                ordered_bricks.push_back(bricks[i]);
            }
        }
    }
    return ordered_bricks;
}

void fill_x_grid(vector<brick_t> bricks)
{
    int zs = x_grid.size();
    for (int i = 0; i < bricks.size(); i++)
    {
        brick_t brick = bricks[i];
        for (int j = brick.x_start; j <= brick.x_end; j++)
        {
            for (int k = brick.z_start; k <= brick.z_end; k++)
            {
                x_grid[zs - k][j].push_back(brick.id);
            }
        }
    }
}

void fill_y_grid(vector<brick_t> bricks)
{
    int zs = y_grid.size();
    for (int i = 0; i < bricks.size(); i++)
    {
        brick_t brick = bricks[i];
        for (int j = brick.y_start; j <= brick.y_end; j++)
        {
            for (int k = brick.z_start; k <= brick.z_end; k++)
            {
                y_grid[zs - k][j].push_back(brick.id);
            }
        }
    }
}

vector<brick_t> fall(vector<brick_t> bricks)
{
    int zs = x_grid.size();
    for (int i = 0; i < bricks.size(); i++)
    {

        brick_t &brick = bricks[i];
        bool can_fall = true;
        while (can_fall)
        {
            if (brick.z_start <= 1)
            { // sol
                can_fall = false;
                break;
            }
            bool x_can_fall = true;
            bool y_can_fall = true;
            bool xy_can_fall = true;
            set<int> x_ids;
            for (int j = brick.x_start; j <= brick.x_end; j++)
            {
                if (x_grid[zs - brick.z_start + 1][j].size() > 0)
                {
                    x_can_fall = false;
                    for (int k = 0; k < x_grid[zs - brick.z_start + 1][j].size(); k++)
                    {
                        x_ids.insert(x_grid[zs - brick.z_start + 1][j][k]);
                    }
                }
            }
            for (int j = brick.y_start; j <= brick.y_end; j++)
            {
                if (y_grid[zs - brick.z_start + 1][j].size() > 0)
                {
                    y_can_fall = false;
                    for (int k = 0; k < y_grid[zs - brick.z_start + 1][j].size(); k++)
                    {
                        if (x_ids.find(y_grid[zs - brick.z_start + 1][j][k]) != x_ids.end())
                        {
                            xy_can_fall = false;
                            break;
                        }
                    }
                }
            }
            if (!xy_can_fall)
            {
                can_fall = false;
                break;
            }
            // if (!x_can_fall && !y_can_fall) {
            //     can_fall = false;
            //     break;
            // }
            if (can_fall)
            {
                brick.z_start--;
                brick.z_end--;
                for (int j = brick.x_start; j <= brick.x_end; j++)
                {
                    x_grid[zs - brick.z_start][j].push_back(brick.id);
                    remove_from_vector(x_grid[zs - brick.z_end - 1][j], brick.id);
                }
                for (int j = brick.y_start; j <= brick.y_end; j++)
                {
                    y_grid[zs - brick.z_start][j].push_back(brick.id);
                    remove_from_vector(y_grid[zs - brick.z_end - 1][j], brick.id);
                }
            }
        }
    }
    return bricks;
}

int second_fall(vector<brick_t> bricks, int rm_index)
{
    vector<vector<vector<int>>> x_grid_copy = x_grid;
    vector<vector<vector<int>>> y_grid_copy = y_grid;
    int counter;
    int zs = x_grid.size();
    for (int i = 0; i < bricks.size(); i++)
    {
        if (i == rm_index - 1)
        {
            continue;
        }
        brick_t &brick = bricks[i];
        bool can_fall = true;
        bool falling = false;
        while (can_fall)
        {
            if (brick.z_start <= 1)
            { // sol
                can_fall = false;
                break;
            }
            bool x_can_fall = true;
            bool y_can_fall = true;
            bool xy_can_fall = true;
            set<int> x_ids;
            for (int j = brick.x_start; j <= brick.x_end; j++)
            {
                if (x_grid_copy[zs - brick.z_start + 1][j].size() > 0)
                {
                    x_can_fall = false;
                    for (int k = 0; k < x_grid_copy[zs - brick.z_start + 1][j].size(); k++)
                    {
                        if (x_grid_copy[zs - brick.z_start + 1][j][k] == rm_index)
                        {
                            continue;
                        }
                        x_ids.insert(x_grid_copy[zs - brick.z_start + 1][j][k]);
                    }
                }
            }
            for (int j = brick.y_start; j <= brick.y_end; j++)
            {
                if (y_grid_copy[zs - brick.z_start + 1][j].size() > 0)
                {
                    y_can_fall = false;
                    for (int k = 0; k < y_grid_copy[zs - brick.z_start + 1][j].size(); k++)
                    {
                        if (y_grid_copy[zs - brick.z_start + 1][j][k] == rm_index)
                        {
                            continue;
                        }
                        if (x_ids.find(y_grid_copy[zs - brick.z_start + 1][j][k]) != x_ids.end())
                        {
                            xy_can_fall = false;
                            break;
                        }
                    }
                }
            }
            if (!xy_can_fall)
            {
                can_fall = false;
                break;
            }
            // if (!x_can_fall && !y_can_fall) {
            //     can_fall = false;
            //     break;
            // }
            if (can_fall)
            {
                brick.z_start--;
                brick.z_end--;
                falling = true;
                for (int j = brick.x_start; j <= brick.x_end; j++)
                {
                    x_grid_copy[zs - brick.z_start][j].push_back(brick.id);
                    remove_from_vector(x_grid_copy[zs - brick.z_end - 1][j], brick.id);
                }
                for (int j = brick.y_start; j <= brick.y_end; j++)
                {
                    y_grid_copy[zs - brick.z_start][j].push_back(brick.id);
                    remove_from_vector(y_grid_copy[zs - brick.z_end - 1][j], brick.id);
                }
            }
        }
        if (falling)
        {
            counter++;
        }
    }
    return counter;
}

bool is_collapsing(int xs1, int xs2, int xe1, int xe2)
{
    if (xs1 >= xs2 && xs1 <= xe2)
    {
        return true;
    }
    if (xe1 >= xs2 && xe1 <= xe2)
    {
        return true;
    }
    if (xs2 >= xs1 && xs2 <= xe1)
    {
        return true;
    }
    if (xe2 >= xs1 && xe2 <= xe1)
    {
        return true;
    }
    return false;
}

bool is_in_vector(vector<brick_t> bricks, brick_t brick)
{
    for (int i = 0; i < bricks.size(); i++)
    {
        if (bricks[i].id == brick.id)
        {
            return true;
        }
    }
    return false;
}

vector<brick_t> get_bricks_on_top(vector<brick_t> bricks, brick_t brick)
{
    vector<brick_t> bricks_on_top;
    for (int i = 0; i < bricks.size(); i++)
    {
        brick_t brick2 = bricks[i];
        if (brick2.id != brick.id)
        {
            if (brick2.z_start == brick.z_end + 1)
            {
                if (is_collapsing(brick.x_start, brick2.x_start, brick.x_end, brick2.x_end) && is_collapsing(brick.y_start, brick2.y_start, brick.y_end, brick2.y_end))
                {
                    bricks_on_top.push_back(brick2);
                }
            }
        }
    }
    return bricks_on_top;
}

vector<brick_t> get_bricks_on_bottom(vector<brick_t> bricks, brick_t brick)
{
    vector<brick_t> bricks_on_bottom;
    for (int i = 0; i < bricks.size(); i++)
    {
        brick_t brick2 = bricks[i];
        if (brick2.id != brick.id)
        {
            if (brick2.z_end == brick.z_start - 1)
            {
                if (is_collapsing(brick.x_start, brick2.x_start, brick.x_end, brick2.x_end) && is_collapsing(brick.y_start, brick2.y_start, brick.y_end, brick2.y_end))
                {
                    bricks_on_bottom.push_back(brick2);
                }
            }
        }
    }
    return bricks_on_bottom;
}



void set_supports(vector<brick_t> bricks) {
    for (int i = 0; i < bricks.size(); i++) {
        brick_t brick = bricks[i];
        vector<brick_t> bricks_on_top = get_bricks_on_top(bricks, brick);
        vector<brick_t> bricks_on_bottom = get_bricks_on_bottom(bricks, brick);
        if (bricks_on_top.size() > 0) {
            set<int> s;
            for (int j = 0; j < bricks_on_top.size(); j++) {
                s.insert(bricks_on_top[j].id);
            }
            supports.insert({brick.id, s});
        } else {
            supports.insert({brick.id, {}});
        }
        if (bricks_on_bottom.size() > 0) {
            set<int> s;
            for (int j = 0; j < bricks_on_bottom.size(); j++) {
                s.insert(bricks_on_bottom[j].id);
            }
            supported_by.insert({brick.id, s});
        } else {
            supported_by.insert({brick.id, {}});
        }
    }
}

int count_falling_after_disintegrate(vector<brick_t> bricks) {
    int zs = x_grid.size();
    int count = 0;
    bricks = order_bricks(bricks);

    for (int i = 0; i < bricks.size(); i++)
    {
        deque<int> q;
        for (auto it = supports[bricks[i].id].begin(); it != supports[bricks[i].id].end(); it++) {
            if (supported_by[*it].size() == 1) {
                q.push_back(*it);
            }
        }
        set<int> falling(q.begin(), q.end());
        falling.insert(bricks[i].id);
        while (!q.empty()) {
            int id = q.front();
            q.pop_front();
            for (auto it = supports[id].begin(); it != supports[id].end(); it++) {
                if (falling.find(*it) == falling.end()) { // not in falling
                    bool can_fall = true;
                    for (auto it2 = supported_by[*it].begin(); it2 != supported_by[*it].end(); it2++) {
                        if (falling.find(*it2) == falling.end()) { // not in falling
                            can_fall = false;
                            break;
                        }
                    }
                    if (can_fall) {
                        q.push_back(*it);
                        falling.insert(*it);
                    }
                }
            }
        }

        count += falling.size() - 1;

    }
    return count;

}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day22/input.txt");
    vector<brick_t> bricks_not_ordered = parse_bricks(input);
    vector<brick_t> bricks = order_bricks(bricks_not_ordered);

    x_grid.resize(max_z);
    for (int i = 0; i < x_grid.size(); i++)
    {
        x_grid[i].resize(max_x + 1);
    }

    y_grid.resize(max_z);
    for (int i = 0; i < y_grid.size(); i++)
    {
        y_grid[i].resize(max_y + 1);
    }

    fill_x_grid(bricks);
    fill_y_grid(bricks);

    vector<brick_t> fallen = fall(bricks);

    set_supports(fallen);

    cout << count_falling_after_disintegrate(fallen) << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}