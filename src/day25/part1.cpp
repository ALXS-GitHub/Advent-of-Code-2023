#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <set>
#include <deque>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

typedef struct input_data_t
{
    string start;
    vector<string> end;
} input_data_t;

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

vector<input_data_t> parse_input(vector<string> input)
{
    vector<input_data_t> parsed_input;
    for (auto line : input)
    {
        input_data_t data;
        data.start = line.substr(0, line.find(":"));
        string end = line.substr(line.find(":") + 2);
        while (end.find(" ") != string::npos)
        {
            data.end.push_back(end.substr(0, end.find(" ")));
            end = end.substr(end.find(" ") + 1);
        }
        data.end.push_back(end);
        parsed_input.push_back(data);
    }
    return parsed_input;
}

// set<vector<string>> get_edges(vector<input_data_t> parsed_input)
// {
//     set<vector<string>> edges;
//     for (auto data : parsed_input)
//     {
//         for (auto end : data.end)
//         {
//             vector<string> edge;
//             edge.push_back(data.start);
//             edge.push_back(end);
//             edges.insert(edge);
//         }
//     }
//     return edges;
// }

unordered_map<string, vector<string>> get_edges(vector<input_data_t> parsed_input)
{
    unordered_map<string, vector<string>> edges;
    for (auto data : parsed_input)
    {
        for (auto end : data.end)
        {
            edges[data.start].push_back(end);
            edges[end].push_back(data.start);
        }
    }
    return edges;
}

// set<string> get_group(string start, vector<input_data_t> parsed_input)
// {
//     set<string> group;
//     group.insert(start);
//     int prev_size = 0;
//     while (prev_size != group.size())
//     {
//         prev_size = group.size();
//         for (auto data : parsed_input)
//         {
//             for (auto end : data.end)
//             {
//                 if (group.find(end) != group.end())
//                 {
//                     group.insert(data.start);
//                 }
//             }
//         }
//     }

//     return group;
// }

int get_cut_group_size(unordered_map<string, vector<string>> edges, string start, vector<string> r1, vector<string> r2, vector<string> r3)
{
    set<string> seen;
    deque<string> to_check;
    // cout << "Start: " << start << r1[0] << r1[1] << r2[0] << r2[1] << r3[0] << r3[1] << endl;

    to_check.push_back(start);
    
    while(to_check.size() > 0)
    {
        string current = to_check.front();
        to_check.pop_front();
        if (seen.find(current) != seen.end())
        {
            continue;
        }
        seen.insert(current);
        for(auto edge : edges[current])
        {
            vector<string> ea = {current, edge};
            if( ea == r1 || ea == r2 || ea == r3)
            {
                continue;
            }
            vector<string> eb = {edge, current};
            if (eb == r1 || eb == r2 || eb == r3)
            {
                continue;
            }
            to_check.push_back(edge);
        }
        
    }
    // cout << "Seen: " << seen.size() << endl;
    return seen.size();
}

int main()
{
    // ! timer
    // auto start_timer = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day25/inputtest.txt");
    vector<input_data_t> parsed_input = parse_input(input);
    unordered_map<string, vector<string>> edges = get_edges(parsed_input);
    string start = edges.begin()->first;

    set<vector<string>> edges_set;

    for (auto e : edges)
    {
        for (auto end : e.second)
        {
            vector<string> edge;
            edge.push_back(e.first);
            edge.push_back(end);
            edges_set.insert(edge);
        }
    }

    // cout << "Edges: " << edges_set.size() << endl;

    int result = 0;
    bool found = false;

    for (auto e1 : edges_set)
    {
        for (auto e2 : edges_set)
        {
            for (auto e3 : edges_set)
            {
                if (e1 == e2 || e1 == e3 || e2 == e3)
                {
                    continue;
                }
                int group_size = get_cut_group_size(edges, start, e1, e2, e3);
                // cout << "Group size: " << group_size << endl;
                if (group_size < edges.size()) {
                    result = group_size;
                    found = true;
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
        if (found)
        {
            break;
        }
    }

    result = (edges.size() - result) * result;
    cout << "Result: " << result << endl;

    // ! timer
    // auto stop_timer = high_resolution_clock::now();
    // auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    // auto seconds = duration.count() / 1000000.0;
    // cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}