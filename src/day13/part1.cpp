#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct pattern_t {
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
    for (int i = 0; i < input.size(); i++) { 
        if (input[i].size() == 0) {
            
            patterns.push_back(pattern);
            pattern.lines.clear();
        } else {
            pattern.lines.push_back(input[i]);
        }
    }
    if (pattern.lines.size() > 0) {
        patterns.push_back(pattern);
    }
    return patterns;
}

bool is_mirror(string a, string b) {
    int min_size = a.size() < b.size() ? a.size() : b.size();
    for (int i = 0; i < min_size; i++) {
        if (a[a.size() - i - 1] != b[i]) {
            return false;
        }
    }   
    return true;
}

int vertical_mirror(pattern_t pattern) {

    for (int j = 0; j < pattern.lines[0].size() - 1; j++){
        bool found = true;
        for (int i = 0; i < pattern.lines.size(); i++) {
            string a = pattern.lines[i].substr(0, j + 1);
            string b = pattern.lines[i].substr(j + 1);
            if (!is_mirror(a, b)) {
                found = false;
                break;
            }
        }
        if (found) {
            return j + 1;
        }
    }
    return 0;
}

int horizontal_mirror(pattern_t pattern) {

    for (int i = 0; i < pattern.lines.size() - 1; i++){
        bool found = true;
        for (int j = 0; j < pattern.lines[i].size(); j++) {
            string a;
            string b;
            for (int k = 0; k < pattern.lines.size(); k++) {
                if (k <= i) {
                    a += pattern.lines[k][j];
                } else {
                    b += pattern.lines[k][j];
                }
            }
            if (!is_mirror(a, b)) {
                found = false;
                break;
            }
        }
        if (found) {
            return i + 1;
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

    for (int i = 0; i < patterns.size(); i++) {
        total += vertical_mirror(patterns[i]) + 100 * horizontal_mirror(patterns[i]);
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