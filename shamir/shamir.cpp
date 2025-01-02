#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <fstream>
#include "json.hpp"

#include <nlohmann/json.hpp> // Use nlohmann/json library for JSON parsing

using json = nlohmann::json;
using namespace std;

// Function to decode the y-value from a given base
long long decodeValue(const string& value, int base) {
    long long result = 0;
    for (char digit : value) {
        result = result * base + (isdigit(digit) ? digit - '0' : digit - 'a' + 10);
    }
    return result;
}

// Function to perform Lagrange Interpolation and find the constant term
long long findConstantTerm(const vector<pair<int, long long>>& points) {
    long long constant = 0;
    int k = points.size();

    for (int i = 0; i < k; ++i) {
        long long term = points[i].second; // Start with the y-value of the point

        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= points[j].first;
                term /= (points[j].first - points[i].first);
            }
        }

        constant += term;
    }

    return constant;
}

int main() {
    // Read JSON input from a file
    ifstream inputFile("testcases.json");
    if (!inputFile.is_open()) {
        cerr << "Failed to open the input file." << endl;
        return 1;
    }

    json testCase;
    inputFile >> testCase;

    // Extract keys
    int n = testCase["keys"]["n"];
    int k = testCase["keys"]["k"];

    vector<pair<int, long long>> points;

    for (auto& [key, value] : testCase.items()) {
        if (key == "keys") continue; // Skip the keys object

        int x = stoi(key);
        int base = stoi(value["base"].get<string>());
        string encodedValue = value["value"].get<string>();

        long long y = decodeValue(encodedValue, base);
        points.emplace_back(x, y);
    }

    // Sort the points based on x (optional but ensures consistent order)
    sort(points.begin(), points.end());

    // Take the first k points for Lagrange Interpolation
    vector<pair<int, long long>> selectedPoints(points.begin(), points.begin() + k);

    // Find the constant term using Lagrange Interpolation
    long long constantTerm = findConstantTerm(selectedPoints);

    cout << "The constant term (secret) is: " << constantTerm << endl;

    return 0;
}