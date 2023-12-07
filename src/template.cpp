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

int main()
{
    // ! timer
    // auto start = high_resolution_clock::now();
    // ! timer





    vector<string> input = read_input("./inputs/day1/input.txt");




    // ! timer
    // auto stop = high_resolution_clock::now();
    // auto duration = duration_cast<microseconds>(stop - start);
    // auto seconds = duration.count() / 1000000.0;
    // cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}