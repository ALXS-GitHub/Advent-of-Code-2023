#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct lens_t {
    string label;
    char symbol;
    int number = -1;
} lens_t;

vector<vector<lens_t>> boxes(256);

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

lens_t get_lens(string s) {
    lens_t lens;
    string temp = "";
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '-' || s[i] == '=') {
            lens.label = temp;
            lens.symbol = s[i];
            temp = "";
        } else {
            temp += s[i];
        }
    }
    if (temp != "") {
        lens.number = stoi(temp);
    }
    return lens;

}

vector<lens_t> split_comma(string line)
{
    vector<lens_t> result;
    string temp = "";
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == ',')
        {
            lens_t lens = get_lens(temp);
            result.push_back(lens);
            temp = "";
        }
        else
        {
            temp += line[i];
        }
    }
    if (temp != "")
    {
        lens_t lens = get_lens(temp);
        result.push_back(lens);
    }
    return result;
}

int calculateHash(string &s)
{
    int hash = 0;
    for (char c : s)
    {
        hash += static_cast<int>(c);
        hash *= 17;
        hash %= 256;
    }
    return hash;
}

void add_to_box(lens_t lens) {
    int hash = calculateHash(lens.label);
    if (lens.symbol == '-') {
        int index = -1;
        for (int i = 0; i < boxes[hash].size(); i++) {
            if (boxes[hash][i].label == lens.label) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            boxes[hash].erase(boxes[hash].begin() + index);
        }
    }
    else if (lens.symbol == '=') {
        int index = -1;
        for (int i = 0; i < boxes[hash].size(); i++) {
            if (boxes[hash][i].label == lens.label) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            boxes[hash][index].number = lens.number;
        } else {
            boxes[hash].push_back(lens);
        }
    }
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day15/input.txt");
    vector<lens_t> list = split_comma(input[0]);
    long long int sum = 0;

    for (lens_t lens : list) {
        add_to_box(lens);
    }

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < boxes[i].size(); j++) {
            sum += (i+1) * (j+1) * (boxes[i][j].number);
        }
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