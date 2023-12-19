#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <set>

using namespace std;
using namespace std::chrono;

typedef struct hole_data_t
{
    char direction;
    long long int distance;
} hole_data_t;

typedef struct vec_t {
    long long int i_start;
    long long int i_end;
    long long int j_start;
    long long int j_end;
} vec_t;

long long int perimeter = 0;
vector<vec_t> trench;

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
        string sub1 = input[i].substr(input[i].find(" ") + 1);
        string sub2 = sub1.substr(sub1.find(" ") + 1);
        string hexa = sub2.substr(2, 5);
        string direction = sub2.substr(7, 1);
        char dir;
        switch (direction[0])
        {
            case '0':
                dir = 'R';
                break;
            case '1':
                dir = 'D';
                break;
            case '2':
                dir = 'L';
                break;
            case '3':
                dir = 'U';
                break;
        }
        long long int dist = stoll(hexa, nullptr, 16);
        hole_data_t hole_data;
        hole_data.direction = dir;
        hole_data.distance = dist;
        data.push_back(hole_data);
    }
    return data;
}

void dig_hole(char direction, long long int distance, long long int &ii, long long int &jj)
{
    vec_t vec;
    if (direction == 'U')
    {
        vec.i_start = ii;
        vec.i_end = ii - distance;
        vec.j_start = jj;
        vec.j_end = jj;
        trench.push_back(vec);
        ii -= distance;
        perimeter += distance;
    }
    else if (direction == 'D')
    {
        vec.i_start = ii;
        vec.i_end = ii + distance;
        vec.j_start = jj;
        vec.j_end = jj;
        trench.push_back(vec);
        ii += distance;
        perimeter += distance;
    }
    else if (direction == 'L')
    {
        vec.i_start = ii;
        vec.i_end = ii;
        vec.j_start = jj;
        vec.j_end = jj - distance;
        trench.push_back(vec);
        jj -= distance;
        perimeter += distance;
    }
    else if (direction == 'R')
    {
        vec.i_start = ii;
        vec.i_end = ii;
        vec.j_start = jj;
        vec.j_end = jj + distance;
        trench.push_back(vec);
        jj += distance;
        perimeter += distance;
    }
       
}

long long int calc_area() {
    long long int area = 0;
    for (int i = 0; i < trench.size(); i++)
    {
        if (trench[i].j_start == trench[i].j_end) {
            continue;
        } else if (trench[i].i_start == trench[i].i_end) {
            area += (trench[i].j_end - trench[i].j_start) * (trench[i].i_end + trench[i].i_start);
        }
    }
    return abs(area) / 2;
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

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day18/input.txt");
    vector<hole_data_t> data = get_data(input);


    long long int ii = 0;
    long long int jj = 0;

    for (int i = 0; i < data.size(); i++)
    {
        dig_hole(data[i].direction, data[i].distance, ii, jj);
    }

    long long int area = calc_area();

    cout << "Total :" << area + perimeter / 2 + 1 << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}