#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <command>" << endl;
        return 1;
    }
    char* program = argv[1];
    clock_t start, end;
    start = clock();
    system(program);
    end = clock();
    cout << "Time elapsed: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}