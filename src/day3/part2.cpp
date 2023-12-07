#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;
vector<char> excluded = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};


typedef struct number_t
{
    string value;
    int i;
    int j;
} number_t;

typedef struct gear_t
{
    int i;
    int j;
    int n1;
    int n2;
} gear_t;

vector<gear_t> gears;

vector<string> read_input(string filename)
{
    vector<string> input;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line.back() == '\r')
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

bool gear_exists(int i, int j)
{
    for (int k = 0; k < gears.size(); k++)
    {
        if (gears[k].i == i && gears[k].j == j)
        {
            return true;
        }
    }
    return false;
}

gear_t& find_gear(int i, int j)
{
    for (gear_t& gear : gears)
    {
        if (gear.i == i && gear.j == j)
        {
            return gear;
        }
    }
}

bool is_in_excluded(char input)
{
    for (int i = 0; i < excluded.size(); i++)
    {
        if (input == excluded[i])
        {
            return true;
        }
    }
    return false;
}

bool has_gear_adjacent(vector<string> input, string number, int i, int j, int max_line_length)
{
    int n = number.size();
    bool ret = false;
    gear_t gear;

    for (int k = j - 1; k <= j + n; k++)
    {
        if (k < 0 || k >= max_line_length)
        {
            continue;
        }
        if (i > 0)
        {
            if (input[i-1].at(k) == '*')
            {
                if (!gear_exists(i-1, k))
                {
                    gear.i = i-1;
                    gear.j = k;
                    gear.n1 = stoi(number);
                    gear.n2 = 0;
                    gears.push_back(gear);
                    return true;
                }
                else
                {
                    gear_t& gear = find_gear(i-1, k);
                    gear.n2 = stoi(number);
                    return true;
                }
            }
        }
        if (i < input.size() - 1)
        {
            if (input[i+1].at(k) == '*')
            {
                if (!gear_exists(i+1, k))
                {
                    gear.i = i+1;
                    gear.j = k;
                    gear.n1 = stoi(number);
                    gear.n2 = 0;
                    gears.push_back(gear);
                    return true;
                }
                else
                {
                    gear_t& gear = find_gear(i+1, k);
                    gear.n2 = stoi(number);
                    return true;
                }
            }
        }
        if (k == j - 1 || k == j + n)
        {
            if (input[i].at(k) == '*')
            {
                if (!gear_exists(i, k))
                {
                    gear.i = i;
                    gear.j = k;
                    gear.n1 = stoi(number);
                    gear.n2 = 0;
                    gears.push_back(gear);
                    return true;
                }
                else
                {
                    gear_t& gear = find_gear(i, k);
                    gear.n2 = stoi(number);
                    return true;
                }
            }
        }
    }
    return false;
}

int has_adjacent(vector<string> input, string number, int i, int j, int max_line_length)
{
    int n = number.size();
    bool ret = false;

    for (int k = j - 1; k <= j + n; k++)
    {
        if (k < 0 || k >= max_line_length)
        {
            continue;
        }
        if (i > 0)
        {
            if (!is_in_excluded(input[i - 1].at(k)))
            {
                return stoi(number);
            }
        }
        if (i < input.size() - 1)
        {
            if (!is_in_excluded(input[i + 1].at(k)))
            {
                return stoi(number);
            }
        }
        if (k == j - 1 || k == j + n)
        {
            if (!is_in_excluded(input[i].at(k)))
            {
                return stoi(number);
            }
        }
    }
    return 0;
}

int main()
{

    // ! timer
    auto start = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day3/input.txt");
    vector<number_t> numbers;
    int max_line_length = input[0].size();

    cout << "Max line length: " << max_line_length << endl;

    for (int i = 0; i < input.size(); i++)
    {
        string buffer = "";
        for (int j = 0; j < max_line_length; j++)
        {
            if (isdigit(input[i].at(j)))
            {
                buffer += input[i].at(j);
            }
            else
            {
                if (buffer.size() > 0)
                {
                    number_t number;
                    number.value = buffer;
                    number.i = i;
                    number.j = j - buffer.size();
                    numbers.push_back(number);
                }
                buffer = "";
            }
            if (j == max_line_length - 1 && buffer.size() > 0)
            {
                number_t number;
                number.value = buffer;
                number.i = i;
                number.j = j - buffer.size() + 1;
                numbers.push_back(number);
            }
        }
    }

    int sum = 0;

    for (int i = 0; i < numbers.size(); i++)
    {
        has_gear_adjacent(input, numbers[i].value, numbers[i].i, numbers[i].j, max_line_length);
    }

    for (int i = 0; i < gears.size(); i++)
    {
        sum += gears[i].n1 * gears[i].n2;
    }

    cout << "Sum: " << sum << endl;

    // ! timer
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}