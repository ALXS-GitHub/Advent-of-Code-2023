#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct race_t {
    int ms;
    int distance;
    int possible = 0;
}race_t;

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

vector<int> get_times(string times) {
    vector<int> times_vector;
    string time;
    for (int i = 0; i < times.size(); i++) {
        if (times[i] == ' ') {
            if (time != "") {
                times_vector.push_back(stoi(time));
                time = "";
            }
        }
        else {
            time += times[i];
        }
    }
    if (time != "") {
        times_vector.push_back(stoi(time));
    }
    return times_vector;
}

vector<int> get_distances(string distances) {
    vector<int> distances_vector;
    string distance;
    for (int i = 0; i < distances.size(); i++) {
        if (distances[i] == ' ') {
            if (distance != "") {
                distances_vector.push_back(stoi(distance));
                distance = "";
            }
        }
        else {
            distance += distances[i];
        }
    }
    if (distance != "") {
        distances_vector.push_back(stoi(distance));
    }
    return distances_vector;
}


vector<race_t> get_races(vector<string>& input) {
    string times = input[0].substr(input[0].find(':') + 1);
    string distances = input[1].substr(input[1].find(':') + 1);

    vector<int> times_vector = get_times(times);
    vector<int> distances_vector = get_distances(distances);

    vector<race_t> races;

    for (int i = 0; i < times_vector.size(); i++) {
        race_t race;
        race.ms = times_vector[i];
        race.distance = distances_vector[i];
        races.push_back(race);
    }

    return races;

}

void get_possibilities(race_t& race) {


    for (int i = 0; i <= race.ms; i++) {
        int distance = 0;
        int remaining = race.ms - i;
        int speed = i;
        for (int j = 0; j < remaining; j++) {
            distance += speed;
        }
        if (distance > race.distance) {
            race.possible++;
        }
    }
}

int main()
{

    // ! timer
    auto start = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day6/input.txt");
    vector<race_t> races = get_races(input);

    cout << input[0] << endl;
    cout << input[1] << endl;


    for (int i = 0; i < races.size(); i++) {
        get_possibilities(races[i]);
    }

    int product = 1;

    for (int i = 0; i < races.size(); i++)
    {
        product *= races[i].possible;
    }

    cout << product << endl;

    // ! timer
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}