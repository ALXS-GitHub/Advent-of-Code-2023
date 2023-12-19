#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <set>

using namespace std;
using namespace std::chrono;

typedef struct hole_data_t {
    char direction;
    int distance;
} hole_data_t;

unordered_map<int, set<int>> trench; // key = i, value = vector of j's

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

vector<hole_data_t> get_data(vector<string> input)
{
    vector<hole_data_t> data;
    for (int i = 0; i < input.size(); i++)
    {
        vector<int> row;
        string direction = input[i].substr(0, 1);
        string distance = input[i].substr(2,3);
        int dist = stoi(distance);
        hole_data_t hole_data;
        hole_data.direction = direction[0];
        hole_data.distance = dist;
        data.push_back(hole_data);
    }
    return data;
}

void dig_hole(char direction, int distance, int &i, int &j)
{
    if (direction == 'U')
    {
        for (int k = 0; k < distance; k++)
        {
            i--;
            trench[i].insert(j);
        }
    }
    else if (direction == 'D')
    {
        for (int k = 0; k < distance; k++)
        {
            i++;
            trench[i].insert(j);
        }
    }
    else if (direction == 'L')
    {
        for (int k = 0; k < distance; k++)
        {
            j--;
            trench[i].insert(j);
        }
    }
    else if (direction == 'R')
    {
        for (int k = 0; k < distance; k++)
        {
            j++;
            trench[i].insert(j);
        }
    }
}

bool vector_contains(vector<int> vec, int value)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == value)
        {
            return true;
        }
    }
    return false;
}

vector<int> find_first_element_to_fill_trench(vector<vector<int>> trench_vec)
{
    vector<int> first_element;
    first_element.push_back(1);
    int j = trench_vec[1][0] + 1;
    first_element.push_back(j);
    return first_element;
}

int fill_trench(vector<vector<bool>> trench_vec_bool, vector<int> start_element)
{
    int count = 0;
    vector<vector<int>> visited;
    vector<vector<int>> new_visited;
    new_visited.push_back(start_element);

    while (new_visited.size() > 0)
    {
        vector<vector<int>> new_new_visited;
        for (int i = 0; i < new_visited.size(); i++)
        {
            int i_ = new_visited[i][0];
            int j_ = new_visited[i][1];
            if (trench_vec_bool[i_ + 1][j_] == false)
            {
                vector<int> new_element;
                new_element.push_back(i_ + 1);
                new_element.push_back(j_);
                new_new_visited.push_back(new_element);
                trench_vec_bool[i_ + 1][j_] = true;
                count++;
            }
            if (trench_vec_bool[i_ - 1][j_] == false)
            {
                vector<int> new_element;
                new_element.push_back(i_ - 1);
                new_element.push_back(j_);
                new_new_visited.push_back(new_element);
                trench_vec_bool[i_ - 1][j_] = true;
                count++;
            }
            if (trench_vec_bool[i_][j_ + 1] == false)
            {
                vector<int> new_element;
                new_element.push_back(i_);
                new_element.push_back(j_ + 1);
                new_new_visited.push_back(new_element);
                trench_vec_bool[i_][j_ + 1] = true;
                count++;
            }
            if (trench_vec_bool[i_][j_ - 1] == false)
            {
                vector<int> new_element;
                new_element.push_back(i_);
                new_element.push_back(j_ - 1);
                new_new_visited.push_back(new_element);
                trench_vec_bool[i_][j_ - 1] = true;
                count++;
            }
        }
        visited.insert(visited.end(), new_visited.begin(), new_visited.end());
        new_visited.clear();
        new_visited = new_new_visited;
    }

    return count;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day18/input.txt");
    vector<hole_data_t> data = get_data(input);
    int ii = 300;
    int jj = 300;

    for (int i = 0; i < data.size(); i++)
    {
        dig_hole(data[i].direction, data[i].distance, ii, jj);
    }

    int min_j = 10000;
    int max_j = -10000;
    for (int i = 0; i < trench.size(); i++)
    {
        for (auto j = trench[i].begin(); j != trench[i].end(); j++)
        {
            if (*j < min_j)
            {
                min_j = *j;
            }
            if (*j > max_j)
            {
                max_j = *j;
            }
        }
    }

    int initial_trench_size = 0;
    vector<vector<int>> trench_vec; // easier access to trench data, with ordered rows
    for (int i = 0; i < trench.size(); i++)
    {
        vector<int> row;
        if (trench[i].size() == 0)
        {
            continue;
        }
        // cout << i << ": ";
        for (auto j = trench[i].begin(); j != trench[i].end(); j++)
        {
            // cout << *j << " ";
            row.push_back(*j - min_j);
            initial_trench_size++;
        }
        trench_vec.push_back(row);
        // cout << endl;
    }

    vector<vector<bool>> trench_vec_bool(trench_vec.size(), vector<bool>(max_j - min_j + 1, false));

    for (int i = 0; i < trench_vec.size(); i++)
    {
        for (int j = 0; j < trench_vec[i].size(); j++)
        {
            trench_vec_bool[i][trench_vec[i][j]] = true;
        }
    }

    vector<int> start_element = find_first_element_to_fill_trench(trench_vec);
    int count = fill_trench(trench_vec_bool, start_element);
    cout << "Count: " << count + initial_trench_size << endl;
    

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}