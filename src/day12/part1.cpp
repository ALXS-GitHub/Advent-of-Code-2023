#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

unordered_map<string, long long int> cache;

typedef struct field_t
{
    string springs;
    vector<int> list;
} field_t;

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

vector<int> get_list(string list)
{
    vector<int> output;
    string number = "";
    for (int i = 0; i < list.size(); i++)
    {
        if (list[i] == ',')
        {
            output.push_back(stoi(number));
            number = "";
        }
        else
        {
            number += list[i];
        }
    }
    output.push_back(stoi(number));
    return output;
}

vector<field_t> get_fields(vector<string> input)
{
    vector<field_t> fields;
    for (int i = 0; i < input.size(); i++)
    {
        field_t field;
        string springs = input[i].substr(0, input[i].find(" "));
        string list = input[i].substr(input[i].find(" ") + 1);
        field.springs = springs;
        field.list = get_list(list);
        fields.push_back(field);
    }
    return fields;
}

vector<int> get_damaged_springs(string springs)
{
    vector<int> output;
    int counter = 0;
    for (int i = 0; i < springs.size(); i++)
    {
        if (springs[i] == '#')
        {
            counter++;
        }
        else
        {
            if (counter > 0)
            {
                output.push_back(counter);
            }
            counter = 0;
        }
    }
    if (counter > 0)
    {
        output.push_back(counter);
    }
    return output;
}

bool is_valid(vector<int> list, string springs)
{

    vector<int> damaged_springs = get_damaged_springs(springs);

    if (damaged_springs != list)
    {
        return false;
    }
    return true;
}

bool in_combinations(vector<vector<char>>& combinations, vector<char>& combination)
{
    for (int i = 0; i < combinations.size(); i++)
    {
        if (combinations[i] == combination)
        {
            return true;
        }
    }
    return false;
}

long long int complete_field(long long int &s, vector<int> &list, string springs, int list_n, int start)
{

    if (cache.count(springs))
    {
        return cache[springs];
    }

    if (list_n >= list.size())
    {
        for (int i = 0; i < springs.size(); i++)
        {
            if (springs[i] == '?')
            {
                springs[i] = '.';
            }
        }
        if (is_valid(list, springs))
        {
            s++;
            cache[springs] = s;
            return s;
        }
        return s;
    }


    if (start + list[list_n] > springs.size()) // check this
    {
        return s;
    }

    int cc = start;
    for (int i = cc; i < springs.size() - list[list_n] + 1; i++) // check this
    {
        string remember;
        remember = springs.substr(i, list[list_n]);
        bool not_possible = false;

        for (int j = i; j < i + list[list_n]; j++)
        {
            if (springs[j] == '.')
            {
                not_possible = true;
                break;
            }
        }

        if (not_possible)
        {
            continue;
        }

        char remember_plus = '/';
        if (i + list[list_n] < springs.size())
        {
            if (springs[i + list[list_n]] == '#')
            {
                not_possible = true;
                if (springs[i] == '?')
                {
                    springs[i] = '.';
                }
                continue;
            }
            remember_plus = springs[i + list[list_n]];
            springs[i + list[list_n]] = '.';
        }

        for (int j = i; j < i + list[list_n]; j++)
        {
            springs[j] = '#';
        }

        complete_field(s, list, springs, list_n + 1, i + list[list_n] + 1); // check this

        for (int j = i; j < i + list[list_n]; j++)
        {
            springs[j] = remember[j - i];
        }

        if (remember_plus != '/')
        {
            springs[i + list[list_n]] = remember_plus;
        }

        if (springs[i] == '?')
        {
            springs[i] = '.';
        }
    }

    cache[springs] = s;
    return s;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer





    vector<string> input = read_input("./inputs/day12/input.txt");
    vector<field_t> fields = get_fields(input);

    int total = 0;
    for (int i = 0; i < fields.size(); i++)
    {
        long long int s = 0;
        string springs = fields[i].springs;
        vector<int> list = fields[i].list;
        int start = 0;
        int list_n = 0;
        complete_field(s, list, springs, list_n, start);
        total += s;
        cache.clear();
    }

    cout << "Total: " << total << endl;


    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}