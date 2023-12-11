#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <map>

using namespace std;
using namespace std::chrono;

//   0
// 3   1
//   2

typedef struct pipedef_t
{
    char name;
    int e1;
    int e2;
} pipedef_t;

typedef struct pipe_t
{
    char name;
    int distance;
    int i;
    int j;
    int e;
    int s;
} pipe_t;

map<char, pipedef_t> pipes;

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

int get_exit(pipe_t pipe)
{
    pipedef_t p = pipes[pipe.name];
    if (pipe.e == p.e1)
    {
        return p.e2;
    }
    else
    {
        return p.e1;
    }
}

int get_exit_from_entry(int e)
{
    switch (e)
    {
    case 0:
        return 2;
    case 1:
        return 3;
    case 2:
        return 0;
    case 3:
        return 1;
    }
    return -1;
}

bool has_connection_with(char pipe, int e)
{
    pipedef_t p = pipes[pipe];
    return p.e1 == e || p.e2 == e;
}

vector<int> get_next_i_j(pipe_t &pipe)
{
    vector<int> next;
    switch (pipe.s)
    {
    case 0:
        next.push_back(pipe.i - 1);
        next.push_back(pipe.j);
        break;
    case 1:
        next.push_back(pipe.i);
        next.push_back(pipe.j + 1);
        break;
    case 2:
        next.push_back(pipe.i + 1);
        next.push_back(pipe.j);
        break;
    case 3:
        next.push_back(pipe.i);
        next.push_back(pipe.j - 1);
        break;
    }
    return next;
}

pipe_t find_next_pipe(vector<string> input, pipe_t &current)
{
    // cout << "Current: " << current.i << ", " << current.j << current.name << current.e << current.s << endl;
    pipe_t next;
    next.distance = current.distance + 1;
    // current.s = get_exit(current);
    next.e = get_exit_from_entry(current.s);
    vector<int> next_i_j = get_next_i_j(current);
    // cout << "Next: " << next_i_j[0] << ", " << next_i_j[1] << endl;
    next.i = next_i_j[0];
    next.j = next_i_j[1];
    next.name = input[next.i][next.j];
    if (next.name == 'S')
    {
        return next;
    }
    next.s = get_exit(next);
    return next;
}

pipe_t find_start(vector<string> input)
{
    pipe_t start;
    start.distance = 0;
    bool found = false;
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            if (input[i][j] == 'S')
            {
                start.i = i;
                start.j = j;
                start.name = 'S';
                start.s = -1;
                start.e = -1;
                found = true;
                break;
            }
        }
        if (found)
        {
            break;
        }
    }
    return start;
}

char find_pipe_at(vector<string> input, int i, int j)
{
    if (i < 0 || i >= input.size())
    {
        return ' ';
    }
    if (j < 0 || j >= input[i].size())
    {
        return ' ';
    }
    return input[i][j];
}

pipe_t find_next_to_start(vector<string> input, pipe_t &start)
{
    // cout << "Start: " << start.i << ", " << start.j << endl;
    pipe_t next;
    int i = start.i + 1;
    int j = start.j;
    char pipe = find_pipe_at(input, i, j);
    if (pipe != ' ' && has_connection_with(pipe, 0))
    {
        next.i = i;
        next.j = j;
        next.name = pipe;
        next.e = 0;
        next.s = get_exit(next);
        next.distance = 1;
        start.s = 2;
        return next;
    }
    i = start.i;
    j = start.j + 1;
    pipe = find_pipe_at(input, i, j);
    if (pipe != ' ' && has_connection_with(pipe, 1))
    {
        next.i = i;
        next.j = j;
        next.name = pipe;
        next.e = 1;
        next.s = get_exit(next);
        next.distance = 1;
        start.s = 3;
        return next;
    }
    i = start.i - 1;
    j = start.j;
    pipe = find_pipe_at(input, i, j);
    if (pipe != ' ' && has_connection_with(pipe, 2))
    {
        next.i = i;
        next.j = j;
        next.name = pipe;
        next.e = 2;
        next.s = get_exit(next);
        next.distance = 1;
        start.s = 0;
        return next;
    }
    i = start.i;
    j = start.j - 1;
    pipe = find_pipe_at(input, i, j);
    if (pipe != ' ' && has_connection_with(pipe, 3))
    {
        next.i = i;
        next.j = j;
        next.name = pipe;
        next.e = 3;
        next.s = get_exit(next);
        next.distance = 1;
        start.s = 1;
        return next;
    }
    cerr << "No next pipe found" << endl;
}

bool is_in_path(vector<pipe_t>& path, int i, int j)
{
    for (int k = 0; k < path.size(); k++)
    {
        if (path[k].i == i && path[k].j == j)
        {
            return true;
        }
    }
    return false;
}

pipe_t get_pipe_in_path(vector<pipe_t>& path, int i, int j)
{
    for (int k = 0; k < path.size(); k++)
    {
        if (path[k].i == i && path[k].j == j)
        {
            return path[k];
        }
    }
    pipe_t p;
    p.name = ' ';
    return p;
}

int get_I_in_column(vector<string>& input, vector<pipe_t>& path, int j)
{
    // int direction_index = 0;
    int horizontal_sections = 0;
    int previous_dir = 0;
    int count = 0;

    for (int i = 0; i < input.size(); i++)
    {
        pipe_t current = get_pipe_in_path(path, i, j);
        if (current.name != ' ') // if we found it
        {
            if (current.name == '-') {
                horizontal_sections++;
            } else if (current.name == '|') {
                continue;    
            }
             else if (previous_dir == 0) {
                if (current.e == 1 || current.s == 1) {
                    previous_dir = 1;
                }
                else {
                    previous_dir = 3;
                }
            } else {
                if (current.e == previous_dir || current.s == previous_dir) {
                    previous_dir = 0;
                    continue;
                } else {
                    previous_dir = 0;
                    horizontal_sections++;
                }
            }
        } else { // cas caractère qui n'est pas dans le path
            if (horizontal_sections % 2 == 1) {
                count++;
            }
        }
        
    }

    return count;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    pipes['|'] = {'|', 0, 2};
    pipes['-'] = {'-', 1, 3};
    pipes['J'] = {'J', 0, 3};
    pipes['L'] = {'L', 0, 1};
    pipes['7'] = {'7', 3, 2};
    pipes['F'] = {'F', 1, 2};

    vector<string> input = read_input("./inputs/day10/input.txt");
    pipe_t start = find_start(input);
    pipe_t next = find_next_to_start(input, start);
    vector<pipe_t> pipes;
    pipes.push_back(next);
    while (next.name != 'S')
    {
        next = find_next_pipe(input, next);
        pipes.push_back(next);
    }

    // set S exit :
    pipes[pipes.size() - 1].s = start.s;

    int sum = 0;

    for (int j = 0; j < input[0].size(); j++)
    {
        int count = get_I_in_column(input, pipes, j);
        // cout << "Column " << j << " : " << count << endl;
        sum += count;
    }

    cout << "Sum : " << sum << endl;


    // ! timer
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}