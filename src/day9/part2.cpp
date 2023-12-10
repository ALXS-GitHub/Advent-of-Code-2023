#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct prediction_t
{
    vector<vector<long long int>> sequence_predictions;
} prediction_t;

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

vector<long long int> parse_line(string line)
{
    vector<long long int> parsed;
    string current = "";
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == ' ')
        {
            parsed.push_back(stoll(current));
            current = "";
        }
        else
        {
            current += line[i];
        }
    }
    if (current != "")
    {
        parsed.push_back(stoll(current));
    }
    return parsed;
}

vector<vector<long long int>> get_sequences(vector<string> input)
{
    vector<vector<long long int>> sequences;
    for (int i = 0; i < input.size(); i++)
    {
        sequences.push_back(parse_line(input[i]));
    }
    return sequences;
}

vector<prediction_t> add_to_predictions(vector<vector<long long int>> sequences)
{
    vector<prediction_t> predictions;
    for (int i = 0; i < sequences.size(); i++)
    {
        prediction_t prediction;
        prediction.sequence_predictions.push_back(sequences[i]);
        predictions.push_back(prediction);
    }
    return predictions;
}

bool is_all_0(vector<long long int> sequence)
{
    for (int i = 0; i < sequence.size(); i++)
    {
        if (sequence[i] != 0)
        {
            return false;
        }
    }
    return true;
}

void add_prediction_history(prediction_t &prediction)
{
    int i = 0;
    while (!is_all_0(prediction.sequence_predictions[i]))
    {
        vector<long long int> new_sequence;
        for (int j = 0; j < prediction.sequence_predictions[i].size() - 1; j++)
        {
            new_sequence.push_back(prediction.sequence_predictions[i][j + 1] - prediction.sequence_predictions[i][j]);
        }
        prediction.sequence_predictions.push_back(new_sequence);
        i++;
    }
}

long long int predict(prediction_t &prediction)
{
    prediction.sequence_predictions[prediction.sequence_predictions.size() - 1].insert(prediction.sequence_predictions[prediction.sequence_predictions.size() - 1].begin(), 0);
    for (int i = prediction.sequence_predictions.size() - 2; i >= 0; i--)
    {
        long long int new_last = prediction.sequence_predictions[i][0] - prediction.sequence_predictions[i + 1][0];
        prediction.sequence_predictions[i].insert(prediction.sequence_predictions[i].begin(), new_last);
    }
    return prediction.sequence_predictions[0][0];
}

int main()
{
    // ! timer
    auto start = high_resolution_clock::now();
    // ! timer

    vector<string> input = read_input("./inputs/day9/input.txt");
    vector<vector<long long int>> sequences = get_sequences(input);
    vector<prediction_t> predictions = add_to_predictions(sequences);

    for (int i = 0; i < predictions.size(); i++)
    {
        add_prediction_history(predictions[i]);
    }

    long long int sum = 0;

    for (int i = 0; i < predictions.size(); i++)
    {
        long long int prediction = predict(predictions[i]);
        // cout << "Prediction: " << prediction << endl;
        sum += prediction;
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