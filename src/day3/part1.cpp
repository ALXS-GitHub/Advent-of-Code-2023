// first attempt on day 3 part 1 (v0)

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
vector<char> excluded = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};

typedef struct number_t
{
    string value;
    int i;
    int j;
} number_t;

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
    vector<string> input = read_input("./inputs/day3/input.txt");
    vector<number_t> numbers;
    int max_line_length = input[0].size();

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
        sum += has_adjacent(input, numbers[i].value, numbers[i].i, numbers[i].j, max_line_length);
    }

    cout << "Sum: " << sum << endl;

    return 0;
}