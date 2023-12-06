#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

typedef struct seed_t
{
    unsigned long long int seed;
} seed_t;

typedef struct map_t {
    int line_start;
    int line_end;
    vector<unsigned long long int> destination;
    vector<unsigned long long int> source;
    vector<unsigned long long int> range;
} map_t;

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

string split_colon(string input)
{
    string output;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == ':')
        {
            output = input.substr(i + 2);
            break;
        }
    }
    return output;
}

vector<unsigned long long int> split_space(string input)
{
    vector<unsigned long long int> output;
    string temp;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == ' ')
        {
            output.push_back(stoul(temp));
            temp = "";
        }
        else
        {
            temp += input[i];
        }
    }
    output.push_back(stoul(temp));

    return output;
}

vector<seed_t> get_seeds(vector<string> input) {
    string line = split_colon(input[0]);
    vector<seed_t> seeds;
    vector<unsigned long long int> values = split_space(line);

    for (int i = 0; i < values.size(); i++) {
        seed_t seed;
        seed.seed = values[i];
        seeds.push_back(seed);
    }

    return seeds;

}

vector<map_t> get_maps(vector<string> input) {
    vector<map_t> maps;
    for (int i = 1; i < input.size(); i++) {
        if (input[i].size() > 0) {
            if (!isdigit(input[i][0])) {
                continue;
            }
            map_t& map = maps.back();
            vector<unsigned long long int> values = split_space(input[i]);
            map.destination.push_back(values[0]);
            map.source.push_back(values[1]);
            map.range.push_back(values[2]);
           
        } else {

            if (maps.size() > 0) {
                map_t& map = maps.back();
                map.line_end = i - 1;
            }

            map_t map;
            map.line_start = i + 2;
            map.line_end = input.size() - 1;
            maps.push_back(map);
        }
        
    }
    return maps;
}

vector<seed_t>& get_seed_location(vector<seed_t>& seeds, vector<map_t>& maps) {

    for (int i = 0; i < seeds.size(); i++) {
        seed_t& seed = seeds[i];
        for (int j = 0; j < maps.size(); j++) {
            map_t& map = maps[j];
            for (int k = 0; k < map.destination.size(); k++) {
                if (seed.seed >= map.source[k] && seed.seed < map.source[k] + map.range[k]) {
                    unsigned long long int delta = seed.seed - map.source[k];
                    seed.seed = map.destination[k] + delta;
                    break;
                }
            }
        }
    
    }

    return seeds;


}

int main()
{
    vector<string> input = read_input("./inputs/day5/input.txt");
    vector<seed_t> seeds = get_seeds(input);
    vector<map_t> maps = get_maps(input);

    vector<seed_t>& seeds_loc = get_seed_location(seeds, maps);

    unsigned long long int min = seeds_loc[0].seed;
 
    for (int i = 0; i < seeds_loc.size(); i++) {
        if (seeds_loc[i].seed < min) {
            min = seeds_loc[i].seed;
        }
    }

    cout << "The minimum seed is " << min << endl;

    return 0;
}