#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<string> read_input(string filename) {
    vector<string> input;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            input.push_back(line);
        }
        cout << "Read " << input.size() << " lines from " << filename << endl;
        file.close();
    }
    return input;
}

int get_number_sum_line(string line) {
    string number = "";
    for (int i = 0; i < line.length(); i++) {
        if (isdigit(line[i])) {
            number += line[i];
            break;
        }
    }
    for (int i = line.length() - 1; i >= 0; i--) {
        if (isdigit(line[i])) {
            number += line[i];
            break;
        }
    }
    int sum = 0;
    sum = stoi(number);
    return sum;

}

int main() {

    // ! timer
    auto start = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day1/input.txt");
    vector<int> numbers;
    int sum; 

    for (int i = 0; i < input.size(); i++) {
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