#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct hand_t
{
    vector<int> cards;
    int bid;
    int value = 0;
} hand_t;

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

vector<hand_t> get_hand(vector<string> input, map<char, int> &cards_map)
{
    vector<hand_t> hands;
    for (int i = 0; i < input.size(); i++)
    {
        hand_t hand;
        string cards = input[i].substr(0, 5);
        string bid = input[i].substr(6);
        hand.bid = stoi(bid);
        for (int j = 0; j < cards.size(); j++)
        {
            char card = cards[j];
            int card_value = cards_map[card];
            hand.cards.push_back(card_value);
        }
        hands.push_back(hand);
    }

    return hands;
}

vector<int> get_first_card(vector<int> cards)
{
    vector<int> first_card;
    for (int i = 0; i < cards.size(); i++)
    {
        if (cards[i] != 1)
        {
            first_card.push_back(cards[i]);
            first_card.push_back(i);
            break;
        }
    }

    if (first_card.size() == 0)
    {
        first_card.push_back(cards[1]);
        first_card.push_back(0);
    }

    return first_card;
}

bool is_five_of_a_kind(hand_t hand)
{
    int card = get_first_card(hand.cards)[0];
    for (int i = 0; i < hand.cards.size(); i++)
    {
        if (i == get_first_card(hand.cards)[1])
        {
            continue;
        }
        if (hand.cards[i] != card && hand.cards[i] != 1)
        {
            return false;
        }
    }
    return true;
}

bool is_foor_of_a_kind(hand_t hand)
{
    vector<int> card_check;
    card_check.push_back(get_first_card(hand.cards)[0]);
    vector<int> count_check;
    count_check.push_back(1);
    int joker_count = 0;

    for (int i = 0; i < hand.cards.size(); i++)
    {
        if (i == get_first_card(hand.cards)[1])
        {
            continue;
        }
        bool found = false;
        for (int j = 0; j < card_check.size(); j++)
        {
            if (hand.cards[i] == 1) {
                joker_count++;
                found = true;
                break;
            }
            if (hand.cards[i] == card_check[j])
            {
                count_check[j]++;
                found = true;
                break;
            }
        }
        if (!found)
        {
            card_check.push_back(hand.cards[i]);
            count_check.push_back(1);
        }
    }

    for (int i = 0; i < count_check.size(); i++)
    {
        if (count_check[i] + joker_count == 4)
        {
            return true;
        }
    }
    return false;
}

bool is_full_house(hand_t hand)
{
    int card = get_first_card(hand.cards)[0];
    int card2 = -1;
    int count1 = 1;
    int count2 = 0;
    int joker_count = 0;
    for (int i = 0; i < hand.cards.size(); i++)
    {
        if (i == get_first_card(hand.cards)[1])
        {
            continue;
        }
        if (hand.cards[i] == 1)
        {
            joker_count++;
            continue;
        }
        else if (hand.cards[i] == card)
        {
            count1++;
            continue;
        }
        else
        {
            if (card2 == -1)
            {
                card2 = hand.cards[i];
                count2++;
            }
            else if (hand.cards[i] == card2)
            {
                count2++;
            }
        }
    }
    return (count1 + joker_count == 3 && count2 == 2) || (count1 == 2 && count2 + joker_count == 3) || (count1 == 3 && count2 == 2) || (count1 == 2 && count2 == 3);
}

bool is_three_of_a_kind(hand_t hand)
{
    vector<int> card_check;
    card_check.push_back(get_first_card(hand.cards)[0]);
    vector<int> count_check;
    count_check.push_back(1);
    int joker_count = 0;

    for (int i = 0; i < hand.cards.size(); i++)
    {
        if (i == get_first_card(hand.cards)[1])
        {
            continue;
        }
        bool found = false;
        for (int j = 0; j < card_check.size(); j++)
        {
            if (hand.cards[i] == 1)
            {
                joker_count++;
                found = true;
                break;
            }
            if (hand.cards[i] == card_check[j])
            {
                count_check[j]++;
                found = true;
                break;
            }
        }
        if (!found)
        {
            card_check.push_back(hand.cards[i]);
            count_check.push_back(1);
        }
    }

    for (int i = 0; i < count_check.size(); i++)
    {
        if (count_check[i] + joker_count == 3)
        {
            return true;
        }
    }
    return false;
}

bool is_two_pairs(hand_t hand)
{
    vector<int> card_check;
    card_check.push_back(get_first_card(hand.cards)[0]);
    vector<int> count_check;
    count_check.push_back(1);
    int joker_count = 0;

    for (int i = 0; i < hand.cards.size(); i++)
    {
        if (i == get_first_card(hand.cards)[1])
        {
            continue;
        }
        bool found = false;
        for (int j = 0; j < card_check.size(); j++)
        {
            if (hand.cards[i] == 1)
            {
                joker_count++;
                found = true;
                break;
            }
            if (hand.cards[i] == card_check[j])
            {
                count_check[j]++;
                found = true;
                break;
            }
        }
        if (!found)
        {
            card_check.push_back(hand.cards[i]);
            count_check.push_back(1);
        }
    }

    int count = 0;
    int max1 = count_check[0];
    int max2 = 0;

    for (int i = 1; i < count_check.size(); i++)
    {
        if (count_check[i] > max1)
        {
            if (max1 > max2)
            {
                max2 = max1;
            }
            max1 = count_check[i];
        }
        else if (count_check[i] > max2)
        {
            max2 = count_check[i];
        }
    }

    return (max1 + joker_count == 2 && max2 == 2) || (max1 == 2 && max2 + joker_count == 2) || (max1 == 2 && max2 == 2);
}

bool is_one_pair(hand_t hand)
{
    vector<int> card_check;
    card_check.push_back(get_first_card(hand.cards)[0]);
    vector<int> count_check;
    count_check.push_back(1);
    int joker_count = 0;
    for (int i = 0; i < hand.cards.size(); i++)
    {
        if (i == get_first_card(hand.cards)[1])
        {
            continue;
        }
        bool found = false;
        for (int j = 0; j < card_check.size(); j++)
        {
            if (hand.cards[i] == 1)
            {
                joker_count++;
                found = true;
                break;
            }
            if (hand.cards[i] == card_check[j])
            {
                count_check[j]++;
                found = true;
                break;
            }
        }
        if (!found)
        {
            card_check.push_back(hand.cards[i]);
            count_check.push_back(1);
        }
    }

    for (int i = 0; i < count_check.size(); i++)
    {
        if (count_check[i] + joker_count == 2)
        {
            return true;
        }
    }

    return false;
}

vector<vector<int>> get_values(vector<hand_t> hands)
{
    vector<vector<int>> values;
    vector<int> five_of_a_kind;
    vector<int> four_of_a_kind;
    vector<int> full_house;
    vector<int> three_of_a_kind;
    vector<int> two_pairs;
    vector<int> one_pair;
    vector<int> high_card;

    for (int i = 0; i < hands.size(); i++)
    {
        hand_t hand = hands[i];
        if (is_five_of_a_kind(hand))
        {
            five_of_a_kind.push_back(i);
        }
        else if (is_foor_of_a_kind(hand))
        {
            four_of_a_kind.push_back(i);
        }
        else if (is_full_house(hand))
        {
            full_house.push_back(i);
        }
        else if (is_three_of_a_kind(hand))
        {
            three_of_a_kind.push_back(i);
        }
        else if (is_two_pairs(hand))
        {
            two_pairs.push_back(i);
        }
        else if (is_one_pair(hand))
        {
            one_pair.push_back(i);
        }
        else
        {
            high_card.push_back(i);
        }
    }

    values.push_back(five_of_a_kind);
    values.push_back(four_of_a_kind);
    values.push_back(full_house);
    values.push_back(three_of_a_kind);
    values.push_back(two_pairs);
    values.push_back(one_pair);
    values.push_back(high_card);

    return values;
}

bool is_better_hand(hand_t hand1, hand_t hand2)
{
    for (int i = 0; i < hand1.cards.size(); i++)
    {
        if (hand1.cards[i] > hand2.cards[i])
        {
            return true;
        }
        else if (hand1.cards[i] < hand2.cards[i])
        {
            return false;
        }
    }
    return false;
}

vector<int> order_type(vector<int> type, vector<hand_t> hands)
{
    vector<int> ordered;

    if (type.size() == 0)
    {
        return ordered;
    }

    ordered.push_back(type[0]);

    for (int i = 1; i < type.size(); i++)
    {
        for (int j = 0; j < ordered.size(); j++)
        {
            if (is_better_hand(hands[type[i]], hands[ordered[j]]))
            {
                ordered.insert(ordered.begin() + j, type[i]);
                break;
            }
            else if (j == ordered.size() - 1)
            {
                ordered.push_back(type[i]);
                break;
            }
        }
    }

    return ordered;
}

int main()
{

    // ! timer
    auto start = high_resolution_clock::now();
    // ! timer

    map<char, int> cards_map;
    cards_map['2'] = 2;
    cards_map['3'] = 3;
    cards_map['4'] = 4;
    cards_map['5'] = 5;
    cards_map['6'] = 6;
    cards_map['7'] = 7;
    cards_map['8'] = 8;
    cards_map['9'] = 9;
    cards_map['T'] = 10;
    cards_map['J'] = 1;
    cards_map['Q'] = 12;
    cards_map['K'] = 13;
    cards_map['A'] = 14;
    vector<string> input = read_input("./inputs/day7/input.txt");

    vector<hand_t> hands = get_hand(input, cards_map);
    vector<vector<int>> values = get_values(hands);

    vector<int> ordered;
    for (int i = 0; i < values.size(); i++)
    {
        vector<int> type = values[i];
        vector<int> ordered_type = order_type(type, hands);
        for (int j = 0; j < ordered_type.size(); j++)
        {
            ordered.push_back(ordered_type[j]);
        }
    }

    int sum_bid = 0;
    for (int i = 0; i < ordered.size(); i++)
    {
        hand_t hand = hands[ordered[i]];
        sum_bid += hand.bid * (ordered.size() - i);
    }

    cout << "Sum bid: " << sum_bid << endl;

    // ! timer
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}