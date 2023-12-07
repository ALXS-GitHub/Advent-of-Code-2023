#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

map<string, string> number_map;

vector<string> read_input(string filename)
{
    vector<string> input;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            input.push_back(line);
        }
        cout << "Read " << input.size() << " lines from " << filename << endl;
        file.close();
    }
    return input;
}

bool is_included(string buffer, string number_str)
{
    if (buffer.find(number_str) != string::npos)
    {
        return true;
    }
    return false;
}

string is_including_number(string buffer)
{
    for (auto const &x : number_map)
    {
        if (is_included(buffer, x.second))
        {
            return x.first;
        }
    }
    return "";
}

int get_number_sum_line(string line)
{
    string number = "";
    string buffer = "";
    for (int i = 0; i < line.length(); i++)
    {
        buffer += line[i];
        if (isdigit(line[i]))
        {
            number += line[i];
            break;
        }
        else if (is_including_number(buffer) != "")
        {
            number += is_including_number(buffer);
            buffer = "";
            break;
        }
    }
    buffer = "";
    for (int i = line.length() - 1; i >= 0; i--)
    {
        buffer = line[i] + buffer;
        if (isdigit(line[i]))
        {
            number += line[i];
            break;
        } else if (is_including_number(buffer) != "")
        {
            number += is_including_number(buffer);
            buffer = "";
            break;
        }
    }
    int sum = 0;
    sum = stoi(number);
    return sum;
}



int main()
{

    // ! timer
    auto start = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day1/input.txt");
    number_map["0"] = "zero";
    number_map["1"] = "one";
    number_map["2"] = "two";
    number_map["3"] = "three";
    number_map["4"] = "four";
    number_map["5"] = "five";
    number_map["6"] = "six";
    number_map["7"] = "seven";
    number_map["8"] = "eight";
    number_map["9"] = "nine";
    
    vector<int> numbers;
    int sum;
    for (int i = 0; i < input.size(); i++)
    {
        int line_sum = get_number_sum_line(input[i]);
        numbers.push_back(line_sum);
        sum += line_sum;
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