#include <string>
#include <algorithm>
#include <iostream>
#include <cstring>

using namespace std;

void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

char **convert_vector_to_array(vector<string> &parsed_command) {
    char **out = new char *[parsed_command.size() + 1];

    for (size_t i = 0; i < parsed_command.size(); ++i) {
        out[i] = strdup(parsed_command[i].c_str());
    }

    out[parsed_command.size()] = nullptr;

    return out;
}

void print_parsed_command(const vector<string> &command) {
    string out;

    for (size_t i = 0; i < command.size(); ++i) {
        cout << "TOKEN " << i << ": " << command[i] << endl;
    }
}

// refactor so that this returns an array
vector<string> parse_command(const string &command) {
    vector<string> out{};

    string currentString;

    for (const auto &token: command) {
        if (!isspace(token)) {
            currentString += token;
        }
        else {
            if (!currentString.empty()) {
                out.push_back(currentString.c_str());
            }
            currentString = "";
        }
    }

    out.push_back(currentString.c_str());

    return out;
}


