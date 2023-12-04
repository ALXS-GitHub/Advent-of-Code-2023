#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
using namespace std::chrono;
using namespace std;


#ifdef _WIN32
    #define COMMAND " > NUL 2>&1"
#else
    #define COMMAND " > /dev/null 2>&1"
#endif

vector<double> test(const char* program, double max_duration)
{
    vector<double> times;
    double cumulated = 0.0;
    while (true)
    {    
        auto start = high_resolution_clock::now();
        system(program);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        auto seconds = duration.count() / 1000000.0;
        times.push_back(seconds);
        cumulated += seconds;
        if (cumulated > max_duration)
            break;
    }
    return times;
}

double mean(vector<double> times)
{
    auto sum = 0.0;
    for (auto x : times)
        sum += x;
    return sum / times.size();
}


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <command>" << endl;
        return 1;
    }
    char* program = argv[1];

    string program_str = program;
    program_str += COMMAND;
    const char *program_cstr = program_str.c_str();

    int result = system(program_cstr);
    if (result != 0)
    {
        // The command returned an error
        cerr << "Error: the command '" << program << "' returned " << result << std::endl;
        return result;
    }

    auto times = test(program_cstr, 1.0);
    cout << "Mean Time: " << mean(times) << "s, Iterations: " << times.size() << endl;
    return 0;
}