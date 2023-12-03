#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define BLUE_MAX 14
#define GREEN_MAX 13
#define RED_MAX 12


using namespace std;

typedef struct game_t {
    int id; 
    int blue = 0;
    int red = 0;
    int green = 0;
} game_t;

vector<string> read_input(string filename)
{
    vector<string> input;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            input.push_back(line);
        }
        cout << "Read " << input.size() << " lines from " << filename << endl;
        file.close();
    }
    return input;
}

vector<string> split(string str, string delim)
{
    vector<string> tokens;
    size_t pos = 0;
    string token;
    while ((pos = str.find(delim)) != string::npos)
    {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delim.length());
    }
    tokens.push_back(str);
    return tokens;
}

game_t parse_game (string line) {
    game_t game;
    string game_id = line.substr(line.find(" "), line.find(":"));
    string game_info = line.substr(line.find(":") + 2);
    vector<string> game_info_sets = split(game_info, "; ");

    for (auto info : game_info_sets) {
        vector<string> set_reveals = split(info, ", ");
        for (auto reveal : set_reveals) {
            if (reveal.find("blue") != string::npos) {
                game.blue = max (game.blue, stoi(reveal.substr(0, reveal.find(" "))));
            } else if (reveal.find("red") != string::npos) {
                game.red = max (game.red, stoi(reveal.substr(0, reveal.find(" "))));
            } else if (reveal.find("green") != string::npos) {
                game.green = max (game.green, stoi(reveal.substr(0, reveal.find(" "))));
            }
        }
    }
    game.id = stoi(game_id);

    return game;
}

bool is_valid_game (game_t game) {
    if (game.blue > BLUE_MAX || game.red > RED_MAX || game.green > GREEN_MAX) {
        return false;
    }
    return true;
}

int main()
{
    vector<string> input = read_input("./inputs/day2/input.txt");

    vector<game_t> games;

    for (auto line : input) {
        games.push_back(parse_game(line));
    }

    int sum = 0;

    for (auto game : games) {
        if (is_valid_game(game)) {
            sum += game.id;
        }
    }

    cout << "Sum of valid games: " << sum << endl;

    return 0;
}