#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <deque>

using namespace std;
using namespace std::chrono;

int low_counter = 0;
int high_counter = 0;

class Module {
    public:
        string name;
        string type;
        vector<string> destinations;
        int state;
        bool has_memory = false;
        unordered_map<string, int> own_memory; // 0 low, 1 high, -1 not set
        Module(string name, string type, vector<string> destinations) {
            this->name = name;
            this->type = type;
            this->destinations = destinations;
            this->state = 0;
            if (type != "%") {
                this->has_memory = true;
            }
        }

        void print() {
            cout << "name: " << this->name << endl;
            cout << "type: " << this->type << endl;
            cout << "state: " << this->state << endl;
            cout << "destinations: ";
            for (int i = 0; i < this->destinations.size(); i++) {
                cout << this->destinations[i] << " ";
            }
            cout << endl;
            cout << "has_memory: " << this->has_memory << endl;
            cout << "own_memory: ";
            for (auto const& [key, val] : this->own_memory) {
                cout << key << ":" << val << " ";
            }
            cout << endl;
        }
};

unordered_map<string, Module*> memory;
vector<string> broadcaster;

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

vector<string> parse_destinations(string destinations)
{
    vector<string> parsed_destinations;
    while (destinations.find(", ") != string::npos)
    {
        string destination = destinations.substr(0, destinations.find(", "));
        parsed_destinations.push_back(destination);
        destinations = destinations.substr(destinations.find(", ") + 2);
    }
    parsed_destinations.push_back(destinations);
    return parsed_destinations;
}

void parse_input(vector<string> input)
{
    for (int i = 0; i < input.size(); i++)
    {
        string line = input[i];
        string current = line.substr(0, line.find(" "));
        string name = current.substr(1);
        string type = current.substr(0, 1);
        string destinations = line.substr(line.find(" ") + 1);
        destinations = destinations.substr(destinations.find(" ") + 1);
        vector<string> parsed_destinations = parse_destinations(destinations);
        if (current == "broadcaster") {
            broadcaster = parsed_destinations;
        } else {
            memory[name] = new Module(name, type, parsed_destinations);
        }
    }
}

void setup() {

    for (auto [k,v] : memory) {
        for (auto destination : v->destinations) {
            if (memory.find(destination) != memory.end()) {
                Module *dest = memory[destination];
                if (dest->type == "&") {
                    dest->own_memory[k] = 0;
                }
            }
        }
    }
}

bool is_all_high(unordered_map<string, int> own_memory) {
    for (auto [k,v] : own_memory) {
        if (v == 0) {
            return false;
        }
    }
    return true;

}

typedef struct queue_data_t {
    string origin;
    string destination;
    int signal; // 0 low, 1 high
} queue_data_t;

long long int do_signals(int range) {
    long long int high = 0;
    long long int low = 0;

    for (int i = 0; i < range; i++) {
        low++;
        deque<queue_data_t> queue;
        for (auto d : broadcaster) {
            queue.push_back({"broadcaster", d, 0});
        }

        while (queue.size() > 0) {
            queue_data_t data = queue.front();
            queue.pop_front();
            if (data.signal == 0) {
                low++;
            } else {
                high++;
            }

            if (memory.find(data.destination) == memory.end()) {
                continue;
            }

            Module *dest = memory[data.destination];

            if (dest->type == "%") {
                if (data.signal == 0) {
                    dest->state = dest->state == 0 ? 1 : 0;
                    int next_signal = dest->state;
                    for (auto d : dest->destinations) {
                        queue.push_back({dest->name, d, next_signal});
                    }
                }
            }
            else {
                dest->own_memory[data.origin] = data.signal;
                int next_signal = is_all_high(dest->own_memory) ? 0 : 1;
                for (auto d : dest->destinations) {
                    queue.push_back({dest->name, d, next_signal});
                }
            }
        }
    }
    return high * low;

}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer





    vector<string> input = read_input("./inputs/day20/input.txt");
    parse_input(input);
    setup();
    long long int result = do_signals(1000);

    cout << "Result: " << result << endl;



    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}