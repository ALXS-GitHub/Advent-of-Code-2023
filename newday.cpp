#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

bool is_number(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (isdigit(s[i]) == false)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        cout << "Usage: newday <day number>" << endl;
        return 1;
    }

    string day = argv[1];

    if (!is_number(day))
    {
        cout << "Usage: newday <day number>" << endl;
        return 1;
    }

    // check if day already exists
    string folder = "./src/day" + day;
    string input_folder = "./inputs/day" + day;
    string filename1 = folder + "/part1.cpp";
    string filename2 = folder + "/part2.cpp";
    string filename3 = input_folder + "/input.txt";
    string filename4 = input_folder + "/inputtest.txt";
    ifstream file1(filename1);
    ifstream file2(filename2);
    ifstream file3(filename3);
    ifstream file4(filename4);
    if (file1.is_open() || file2.is_open() || file3.is_open() || file4.is_open())
    {
        cout << "Day " << day << " already exists" << endl;
        return 1;
    }

    // create folder
    fs::create_directory(folder);
    fs::create_directory(input_folder);

    // create files
    // part1.cpp (copy template.cpp)
    fs::copy("./src/template.cpp", filename1);
    // part2.cpp empty
    ofstream file(filename2);
    file.close();
    // input.txt empty
    ofstream file5(filename3);
    file5.close();
    // inputtest.txt empty
    ofstream file6(filename4);
    file6.close();

    cout << "Created day " << day << endl;

    return 0;

}