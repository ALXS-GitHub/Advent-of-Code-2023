#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <tuple>

using namespace std;
using namespace std::chrono;

typedef struct pattern_t
{
    vector<string> lines;
} pattern_t;

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

vector<pattern_t> parse_input(vector<string> input)
{
    vector<pattern_t> patterns;
    pattern_t pattern;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i].size() == 0)
        {

            patterns.push_back(pattern);
            pattern.lines.clear();
        }
        else
        {
            pattern.lines.push_back(input[i]);
        }
    }
    if (pattern.lines.size() > 0)
    {
        patterns.push_back(pattern);
    }
    return patterns;
}

bool is_mirror(string a, string b)
{
    int min_size = a.size() < b.size() ? a.size() : b.size();
    for (int i = 0; i < min_size; i++)
    {
        if (a[a.size() - i - 1] != b[i])
        {
            return false;
        }
    }
    return true;
}

int vertical_mirror(pattern_t pattern, int avoid)
{

    for (int j = 0; j < pattern.lines[0].size() - 1; j++)
    {
        if (j == avoid - 1)
        {
            continue;
        }
        bool found = true;
        for (int i = 0; i < pattern.lines.size(); i++)
        {
            string a = pattern.lines[i].substr(0, j + 1);
            string b = pattern.lines[i].substr(j + 1);
            if (!is_mirror(a, b))
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return j + 1;
        }
    }
    return 0;
}

int horizontal_mirror(pattern_t pattern, int avoid)
{

    for (int i = 0; i < pattern.lines.size() - 1; i++)
    {
        if (i == avoid - 1)
        {
            continue;
        }
        bool found = true;
        for (int j = 0; j < pattern.lines[i].size(); j++)
        {
            string a;
            string b;
            for (int k = 0; k < pattern.lines.size(); k++)
            {
                if (k <= i)
                {
                    a += pattern.lines[k][j];
                }
                else
                {
                    b += pattern.lines[k][j];
                }
            }
            if (!is_mirror(a, b))
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return i + 1;
        }
    }

    return 0;
}

vector<int> get_value(pattern_t pattern, vector<int> original)
{
    // for (int i = 0; i < pattern.lines.size(); i++)
    // {
    //     for (int j = 0; j < pattern.lines[0].size(); j++)
    //     {
    //         cout << pattern.lines[i][j];
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    vector<int> result = {0,0};
    int v = vertical_mirror(pattern, original[0]);
    if (v > 0 && v != original[0])
    {
        result[0] = v;
    }
    int h = horizontal_mirror(pattern, original[1]);
    if (h > 0 && h != original[1])
    {
        result[1] = h;
    }

    return result;
}

int get_value_smudged(pattern_t pattern)
{

    vector<int> original = get_value(pattern, {-1, -1});

    for (int i = 0; i < pattern.lines.size(); i++)
    {
        for (int j = 0; j < pattern.lines[0].size(); j++)
        {
            if (pattern.lines[i][j] == '#')
            {
                pattern.lines[i][j] = '.';
                vector<int> nv = get_value(pattern, original);
                if (nv != vector<int>{0, 0} && nv != original)
                {
                    if (nv[0] > 0)
                    {
                        return nv[0];
                    }
                    else
                    {
                        return nv[1] * 100;
                    }
                }
                pattern.lines[i][j] = '#';
            }
            else if (pattern.lines[i][j] == '.') {
                pattern.lines[i][j] = '#';
                vector<int> nv = get_value(pattern, original);
                if (nv != vector<int>{0, 0} && nv != original)
                {
                    if (nv[0] > 0)
                    {
                        return nv[0];
                    }
                    else
                    {
                        return nv[1] * 100;
                    }
                }
                pattern.lines[i][j] = '.';
            } 
            
        }
    }
    return 0;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day13/input.txt");
    vector<pattern_t> patterns = parse_input(input);

    long long int total = 0;

    for (int i = 0; i < patterns.size(); i++)
    {
        total += get_value_smudged(patterns[i]);
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