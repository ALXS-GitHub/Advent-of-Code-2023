#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

vector<string> split_comma(string line) {
    vector<string> result;
    string temp = "";
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ',') {
            result.push_back(temp);
            temp = "";
        } else {
            temp += line[i];
        }
    }
    if (temp != "") {
        result.push_back(temp);
    }
    return result;

}

int calculateHash(string& s) {
    int hash = 0;
    for (char c : s)
    {
        hash += static_cast<int>(c);
        hash *= 17;
        hash %= 256;
    }
    return hash;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer



    vector<string> input = read_input("./inputs/day15/input.txt");
    vector<string> list = split_comma(input[0]);
    long long int sum = 0;

    for (string s : list) {
        sum += calculateHash(s);
    }

    cout << "Sum: " << sum << endl;


    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}