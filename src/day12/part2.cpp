#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <unordered_map>
#include <tuple>

using namespace std;
using namespace std::chrono;

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

long long int nb_possible(string springs, vector<int> list) {
    list.push_back(0);
    springs = springs + '.';
    int max_list = 0;
    for (int i = 0; i < list.size(); i++) {
        if (list[i] > max_list) {
            max_list = list[i];
        }
    }

    vector<vector<vector<long long int>>> dp(springs.size(), vector<vector<long long int>>(list.size(), vector<long long int>(max_list + 1, 0)));

    for (int i = 0; i < springs.size(); i++) {
        for (int j = 0; j < list.size(); j++) {
            for (int k = 0; k <= list[j]; k++) {
                if (i == 0) {
                    if (j != 0) {
                        dp[i][j][k] = 0;
                        continue;
                    }

                    if (springs[i] == '#') {
                        if (k != 1) {
                            dp[i][j][k] = 0;
                            continue;
                        }
                        dp[i][j][k] = 1;
                        continue;
                    }

                    if (springs[i] == '.') {
                        if (k != 0) {
                            dp[i][j][k] = 0;
                            continue;
                        }
                        dp[i][j][k] = 1;
                        continue;
                    }

                    if (springs[i] == '?') {
                        if (k != 1 && k != 0) {
                            dp[i][j][k] = 0;
                            continue;
                        }
                        dp[i][j][k] = 1;
                        continue;
                    }

                }
                long long int a1;
                long long int a2;
                if (k != 0) {
                    a1 = 0;
                } else if (j > 0) {
                    if (k == 0) {
                        a1 = dp[i - 1][j - 1][list[j - 1]] + dp[i - 1][j][0];
                    }
                } else
                {
                    string sub = springs.substr(0, i);
                    if (sub.find('#') != string::npos)
                    {
                        a1 = 0;
                    }
                    else
                    {
                        a1 = 1;
                    }
                }
                if ( k == 0) {
                    a2 = 0;
                } else {
                    a2 = dp[i - 1][j][k - 1];
                }
                if (springs[i] == '#') {
                    dp[i][j][k] = a2;
                } else if (springs[i] == '.') {
                    dp[i][j][k] = a1;
                } else {
                    dp[i][j][k] = a1 + a2;
                }
            }
        }
    }

    long long int result = dp[springs.size()-1][list.size() - 1][0];
    return result;
}

vector<field_t> rearengement(vector<field_t> fields, vector<long long int> arengement)
{
    vector<field_t> output;
    for (int i = 0; i < fields.size(); i++)
    {
        string springs_copy = fields[i].springs;
        vector<int> list_copy = fields[i].list;
        field_t field;
        field.springs = fields[i].springs;
        field.list = fields[i].list;

        field.springs = field.springs + '?';
        field.springs = field.springs + springs_copy;
        field.list.insert(field.list.end(), list_copy.begin(), list_copy.end());
        field.springs = field.springs + '?';
        field.springs = field.springs + springs_copy;
        field.list.insert(field.list.end(), list_copy.begin(), list_copy.end());
        field.springs = field.springs + '?';
        field.springs = field.springs + springs_copy;
        field.list.insert(field.list.end(), list_copy.begin(), list_copy.end());
        field.springs = field.springs + '?';
        field.springs = field.springs + springs_copy;
        field.list.insert(field.list.end(), list_copy.begin(), list_copy.end());

        output.push_back(field);
    }
    return output;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day12/input.txt");
    vector<field_t> fields = get_fields(input);
    vector<field_t> fields_copy = fields;
    vector<long long int> arengements;
    vector<long long int> arengements_second;

    vector<field_t> rearenged_fields = rearengement(fields_copy, arengements);

    long long int total = 0;
    for (int i = 0; i < rearenged_fields.size(); i++)
    {
        long long int s = 0;
        string springs = rearenged_fields[i].springs;
        vector<int> list = rearenged_fields[i].list;
        int start = 0;
        int list_n = 0;
        s = nb_possible(springs, list);
        total += s;
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