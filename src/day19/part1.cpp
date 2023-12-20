#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

typedef struct rating_t {
    int x;
    int m;
    int a;
    int s;
} rating_t;

typedef struct rule_t {
    char category;
    char symbol;
    int number;
    string go_to;
} rule_t;

typedef struct workflow_t {
    string name;
    vector<rule_t> rules;
    string end;
} workflow_t;

unordered_map<string, workflow_t> workflows_map;

vector<vector<string>> read_input(string filename)
{
    vector<vector<string>> input;
    vector<string> workflows;
    vector<string> ratings;
    bool reading_workflows = true;
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
            if (line.size() == 0)
            {
                reading_workflows = false;
            }
            else if (reading_workflows)
            {
                workflows.push_back(line);
            }
            else
            {
                ratings.push_back(line);
            }
        }
        input.push_back(workflows);
        input.push_back(ratings);
        cout << "Read " << workflows.size() + ratings.size() + 1 << " lines from " << filename << endl;
        file.close();
    }
    return input;
}

vector<workflow_t> parse_workflows(vector<string> workflows)
{
    vector<workflow_t> parsed_workflows;
    for (int i = 0; i < workflows.size(); i++)
    {
        workflow_t workflow;
        string line = workflows[i];
        workflow.name = line.substr(0, line.find("{"));
        line = line.substr(line.find("{") + 1);
        while (line.find(",") != string::npos)
        {
            rule_t rule;
            string rule_line = line.substr(0, line.find(","));
            rule.category = rule_line[0];
            rule.symbol = rule_line[1];
            rule.number = stoi(rule_line.substr(2, rule_line.find(":")));
            rule.go_to = rule_line.substr(rule_line.find(":") + 1);
            workflow.rules.push_back(rule);
            line = line.substr(line.find(",") + 1);
        }
        workflow.end = line.substr(0, line.find("}"));
        parsed_workflows.push_back(workflow);
    }
    return parsed_workflows;
}

void parse_workflows_to_map(vector<workflow_t> workflows)
{
    for (int i = 0; i < workflows.size(); i++)
    {
        workflow_t workflow = workflows[i];
        workflows_map[workflow.name] = workflow;
    }
}

vector<rating_t> parse_ratings(vector<string> ratings)
{
    vector<rating_t> parsed_ratings;
    for (int i = 0; i < ratings.size(); i++)
    {
        rating_t rating;
        string line = ratings[i];
        rating.x = stoi(line.substr(line.find("=") + 1, line.find(",")));
        line = line.substr(line.find(",") + 1);
        rating.m = stoi(line.substr(line.find("=") + 1, line.find(",")));
        line = line.substr(line.find(",") + 1);
        rating.a = stoi(line.substr(line.find("=") + 1, line.find(",")));
        line = line.substr(line.find(",") + 1);
        rating.s = stoi(line.substr(line.find("=") + 1, line.find("}")));
        parsed_ratings.push_back(rating);
    }
    return parsed_ratings;
}

bool is_accepted(rating_t rating, string workflow_name) {

    if (workflow_name == "R") {
        return false;
    }

    if (workflow_name == "A") {
        return true;
    }

    workflow_t workflow = workflows_map[workflow_name];
    for (int i = 0; i < workflow.rules.size(); i++) {
        rule_t rule = workflow.rules[i];
        if (rule.category == 'x') {
            if (rule.symbol == '>') {
                if (rating.x > rule.number) {
                    return is_accepted(rating, rule.go_to);
                }
            } else if (rule.symbol == '<') {
                if (rating.x < rule.number) {
                    return is_accepted(rating, rule.go_to);
                }
            }
        } else if (rule.category == 'm') {
            if (rule.symbol == '>') {
                if (rating.m > rule.number) {
                    return is_accepted(rating, rule.go_to);
                }
            } else if (rule.symbol == '<') {
                if (rating.m < rule.number) {
                    return is_accepted(rating, rule.go_to);
                }
            }
        } else if (rule.category == 'a') {
            if (rule.symbol == '>') {
                if (rating.a > rule.number) {
                    return is_accepted(rating, rule.go_to);
                }
            } else if (rule.symbol == '<') {
                if (rating.a < rule.number) {
                    return is_accepted(rating, rule.go_to);
                }
            }
        } else if (rule.category == 's') {
            if (rule.symbol == '>') {
                if (rating.s > rule.number) {
                    return is_accepted(rating, rule.go_to);
                }
            } else if (rule.symbol == '<') {
                if (rating.s < rule.number) {
                    return is_accepted(rating, rule.go_to);
                }
            }
        }
    }

    if (workflow.end == "A") {
        return true;
    } else if (workflow.end == "R") {
        return false;
    }



    return is_accepted(rating, workflow.end);
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer





    vector<vector<string>> input = read_input("./inputs/day19/input.txt");
    vector<string> workflows = input[0];
    vector<string> ratings = input[1];

    vector<workflow_t> parsed_workflows = parse_workflows(workflows);
    parse_workflows_to_map(parsed_workflows);
    vector<rating_t> parsed_ratings = parse_ratings(ratings);

    vector<rating_t> accepted_ratings;

    for (int i = 0; i < parsed_ratings.size(); i++) {
        rating_t rating = parsed_ratings[i];
        if (is_accepted(rating, "in")) {
            accepted_ratings.push_back(rating);
        }
    }

    long long int total = 0;

    for (int i = 0; i < accepted_ratings.size(); i++) {
        rating_t rating = accepted_ratings[i];
        total += rating.s + rating.a + rating.m + rating.x;
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