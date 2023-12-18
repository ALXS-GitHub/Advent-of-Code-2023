#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

//   0  
// 3   1
//   2

typedef struct beam_t {
    int index;
    int i;
    int j;
    int direction;
    bool to_remove = false;
} beam_t;

int global_index = 0;
vector<beam_t> beams;

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

void beam_next(beam_t& beam) {
    if (beam.direction == 0)
    {
        beam.i--;
    }
    else if (beam.direction == 1)
    {
        beam.j++;
    }
    else if (beam.direction == 2)
    {
        beam.i++;
    }
    else if (beam.direction == 3)
    {
        beam.j--;
    }
}

void erases_wrong_beams(vector<beam_t> &beams, vector<string> &input)
{
    int i = 0;
    while (i < beams.size())
    {
        if (beams[i].i < 0 || beams[i].i >= input.size() || beams[i].j < 0 || beams[i].j >= input[0].size())
        {
            beams.erase(beams.begin() + i);
        }
        else if (beams[i].to_remove)
        {
            beams.erase(beams.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void reflect_beam(int idx, vector<string>& input, vector<vector<vector<bool>>> &energized, vector<beam_t>& beams, long long int& count_manager)
{
    if (beams[idx].i < 0 || beams[idx].i >= input.size() || beams[idx].j < 0 || beams[idx].j >= input[0].size())
    {
        // cout << "Beam " << beams[idx].index << " out of bounds" << endl;
        beams.erase(beams.begin() + idx);
        return;
    }

    if (energized[beams[idx].i][beams[idx].j][beams[idx].direction])
    {
        beams[idx].to_remove = true;
    } else 
    {
        count_manager++;
        energized[beams[idx].i][beams[idx].j][beams[idx].direction] = true;
    }

    if (input[beams[idx].i][beams[idx].j] == '/')
    {
        if (beams[idx].direction == 0)
        {
            beams[idx].direction = 1;
        }
        else if (beams[idx].direction == 1)
        {
            beams[idx].direction = 0;
        }
        else if (beams[idx].direction == 2)
        {
            beams[idx].direction = 3;
        }
        else if (beams[idx].direction == 3)
        {
            beams[idx].direction = 2;
        }
    }
    else if (input[beams[idx].i][beams[idx].j] == '\\')
    {
        if (beams[idx].direction == 0)
        {
            beams[idx].direction = 3;
        }
        else if (beams[idx].direction == 1)
        {
            beams[idx].direction = 2;
        }
        else if (beams[idx].direction == 2)
        {
            beams[idx].direction = 1;
        }
        else if (beams[idx].direction == 3)
        {
            beams[idx].direction = 0;
        }
    }
    else if (input[beams[idx].i][beams[idx].j] == '-')
    {
        if (beams[idx].direction == 0)
        {
            global_index++;
            beam_t new_beam = {global_index, beams[idx].i, beams[idx].j, 1};
            beam_next(new_beam);
            beams.push_back(new_beam);
            beams[idx].direction = 3;
        }
        else if (beams[idx].direction == 2)
        {
            global_index++;
            beam_t new_beam = {global_index, beams[idx].i, beams[idx].j, 1};
            beam_next(new_beam);
            beams.push_back(new_beam);
            beams[idx].direction = 3;
        }
    }
    else if (input[beams[idx].i][beams[idx].j] == '|')
    {
        if (beams[idx].direction == 1)
        {

            global_index++;
            beam_t new_beam = {global_index, beams[idx].i, beams[idx].j, 2};
            beam_next(new_beam);      
            beams.push_back(new_beam);
            beams[idx].direction = 0;
        }
        else if (beams[idx].direction == 3)
        {   
            global_index++;
            beam_t new_beam = {global_index, beams[idx].i, beams[idx].j, 2};
            beam_next(new_beam);
            beams.push_back(new_beam);
            beams[idx].direction = 0;
        }
    }
    beam_next(beams[idx]);
}

long long int count_energized(vector<vector<vector<bool>>> &energized)
{
    long long int count = 0;
    for (int i = 0; i < energized.size(); i++)
    {
        for (int j = 0; j < energized[i].size(); j++)
        {
            for (int k = 0; k < energized[i][j].size(); k++)
            {
                if (energized[i][j][k])
                {
                    count++;
                    break;
                }
            }
        }
    }
    return count;
}

long long int count_full_energized(vector<vector<vector<bool>>> &energized)
{
    long long int count = 0;
    for (int i = 0; i < energized.size(); i++)
    {
        for (int j = 0; j < energized[i].size(); j++)
        {
            for (int k = 0; k < energized[i][j].size(); k++)
            {
                if (energized[i][j][k])
                {
                    count++;
                }
            }
        }
    }
    return count;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer





    vector<string> input = read_input("./inputs/day16/input.txt");
    vector<vector<vector<bool>>> energized(input.size(), vector<vector<bool>>(input[0].size(), vector<bool>(4, false)));
    beams.push_back({0, 0, 0, 1});
    global_index++;
    long long int prev_count_manager = -1;
    long long int count_manager = 0;
    long long int energized_count = count_full_energized(energized);

    while (prev_count_manager != count_manager)
    {
        prev_count_manager = count_manager;
        // cout << "Beams: " << beams.size() << endl;
        // cout << "Beams: " << beams.size() << endl;
        for (int i = 0; i < beams.size(); i++)
        {
            // cout << beams[i].i << " " << beams[i].j << " " << beams[i].direction << endl;
            reflect_beam(i, input, energized, beams, count_manager);
        }
        erases_wrong_beams(beams, input);
        // cout << prev_count_manager << " " << count_manager << endl;
    }

    cout << "Energized: " << count_energized(energized) << endl;

    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}