#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct card_t {
    vector<int> winning;
    vector<int> numbers;
} card_t;

vector<string> read_input(string filename)
{
    vector<string> input;
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line.back() == '\r')
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

vector<int> split_card(string str, string delim)
{
    vector<int> tokens;
    size_t pos = 0;
    string token;
    while ((pos = str.find(delim)) != string::npos)
    {
        token = str.substr(0, pos);
        if (!token.empty())
        {
            tokens.push_back(stoi(token));
        }
        str.erase(0, pos + delim.length());
    }
    tokens.push_back(stoi(str));
    return tokens;
}

card_t parse_card(string line) {
    card_t card;
    string game_info = line.substr(line.find(":") + 2);
    string game_winning = game_info.substr(0, game_info.find(" | "));
    string game_numbers = game_info.substr(game_info.find(" | ") + 3);

    vector<int> winning = split_card(game_winning, " ");
    vector<int> numbers = split_card(game_numbers, " ");

    card.winning = winning;
    card.numbers = numbers;

    return card;
}

bool is_winning_number(int number, vector<int> winning) {
    for (auto win : winning) {
        if (number == win) {
            return true;
        }
    }
    return false;
}

int card_points(card_t card) {
    int points = 0;
    int coef = 2;
    for (int i = 0; i < card.numbers.size(); i++) {
        if (is_winning_number(card.numbers[i], card.winning)) {
            if (points == 0) {
                points = 1;
            } else {
                points *= coef;
            }
        }
    }
    return points;
}

int main()
{

    // ! timer
    auto start = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day4/input.txt");
    vector<card_t> cards;

    for (auto line : input) {
        cards.push_back(parse_card(line));
    }

    int sum = 0;
    for (auto card : cards) {
        sum += card_points(card);
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