#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

const double MIN = 200000000000000;
const double MAX = 400000000000000;
// const double MIN = 7;
// const double MAX = 27;

typedef struct hailstone_t
{
    int id;
    double px;
    double py;
    double pz;
    double vx;
    double vy;
    double vz;
} hailstone_t;

struct Point
{
    double x, y;
};

// Point intersect(Point p1, Point p2, Point p3, Point p4)
// {
//     double denom = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
//     if (denom == 0)
//         throw std::runtime_error("Lines are parallel");

//     double t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / denom;
//     return {p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y)};
// }

Point intersect(hailstone_t h1, hailstone_t h2)
{
    double denom = h1.vx * h2.vy - h1.vy * h2.vx;
    if (denom == 0)
        return {-1, -1};

    double b1 = h1.px - h2.px;
    double b2 = h1.py - h2.py;
    double s = (- h2.vy * b1 + h2.vx * b2) / denom;
    return {h1.px + s * h1.vx, h1.py + s * h1.vy};
}

vector<string>
read_input(string filename)
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

vector<hailstone_t> parse_input(vector<string> input)
{
    vector<hailstone_t> hailstones;
    for (int i = 0; i < input.size(); i++)
    {
        string line = input[i];
        int id = i;
        string pos = line.substr(0, line.find("@") - 1);
        string vel = line.substr(line.find("@") + 2);
        string px = pos.substr(0, pos.find(","));
        pos = pos.substr(pos.find(",") + 2);
        string py = pos.substr(0, pos.find(","));
        pos = pos.substr(pos.find(",") + 2);
        string pz = pos;
        string vx = vel.substr(0, vel.find(","));
        vel = vel.substr(vel.find(",") + 2);
        string vy = vel.substr(0, vel.find(","));
        vel = vel.substr(vel.find(",") + 2);
        string vz = vel;
        hailstone_t h = {id, stod(px), stod(py), stod(pz), stod(vx), stod(vy), stod(vz)};
        hailstones.push_back(h);
    }
    return hailstones;
}

bool valid_intersection(hailstone_t h1, hailstone_t h2) {
    Point p = intersect(h1, h2);
    if (h1.vx < 0) {
        if (p.x > h1.px) {
            return false;
        }
    } else {
        if (p.x < h1.px) {
            return false;
        }
    }
    if (h1.vy < 0) {
        if (p.y > h1.py) {
            return false;
        }
    } else {
        if (p.y < h1.py) {
            return false;
        }
    }
    if (h2.vx < 0) {
        if (p.x > h2.px) {
            return false;
        }
    } else {
        if (p.x < h2.px) {
            return false;
        }
    }
    if (h2.vy < 0) {
        if (p.y > h2.py) {
            return false;
        }
    } else {
        if (p.y < h2.py) {
            return false;
        }
    }

    if (p.x >= MIN && p.x <= MAX && p.y >= MIN && p.y <= MAX) {
        // cout << "Intersection at " << p.x << ", " << p.y << endl;
        return true;
    }
    return false;
}

long long int hailstone_do_intersect(vector<hailstone_t> hailstones) {
    long long int count = 0;
    for (int i = 0; i < hailstones.size(); i++) {
        for (int j = i + 1; j < hailstones.size(); j++) {
            if (valid_intersection(hailstones[i], hailstones[j])) {
                count++;
            }
        }
    }
    return count;
}

int main()
{
    // ! timer
    auto start_timer = high_resolution_clock::now();
    // ! timer





    vector<string> input = read_input("./inputs/day24/input.txt");

    // * intersection tests
    // hailstone_t h1 = {1, 19, 13, 30, -2, 1, -2};
    // hailstone_t h2 = {2, 18, 19, 22, -1, -1, -2};
    // hailstone_t h3 = {1, 19, 13, 30, -2, 1, -2};
    // hailstone_t h4 = {2, 20, 25, 34, -2, -2, -4};
    // hailstone_t h5 = {1, 1, 0, 3, -1, 1, -2};
    // hailstone_t h6 = {2, 1, -1, 4, 2, -1, 3};

    // Point p = intersect(h5, h6);
    // cout << "Intersection at " << p.x << ", " << p.y << endl;

    parse_input(input);
    // for (auto h : parse_input(input))
    // {
    //     cout << h.id << " " << h.px << " " << h.py << " " << h.pz << " " << h.vx << " " << h.vy << " " << h.vz << endl;
    // }

    cout << "Intersections: " << hailstone_do_intersect(parse_input(input)) << endl;




    // ! timer
    auto stop_timer = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop_timer - start_timer);
    auto seconds = duration.count() / 1000000.0;
    cout << "Time " << seconds << " seconds" << endl;
    // ! timer

    return 0;
}