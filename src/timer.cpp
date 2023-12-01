#include <iostream>
#include <iomanip>
#include <chrono>
using namespace std::chrono;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <command>" << endl;
        return 1;
    }
    char* program = argv[1];
    auto start = high_resolution_clock::now();
    system(program);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time elapsed: " << fixed << setprecision(6) << seconds << "s" << endl;
    return 0;
}