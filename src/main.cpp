#include <iostream>
#include "../headers/util.h"
#include <vector>
#include <cctype>

using namespace std;

vector<string> parse_command(const string & command);
void print_parsed_command(const vector<string> &command);

int main() {
    string input;

    while (!cin.eof() && input != "exit") {
        cout << "> ";

        getline(cin, input);

        vector<string> parsed_command = parse_command(input);
    }

    return 0;
}

void print_parsed_command(const vector<string> &command) {
    string out;

    for (int i = 0; i < command.size() - 1; i++) {
        out += command[i] + " ";
    }

    out += command.back();

    cout << out << endl;
}

vector<string> parse_command(const string &command) {
    vector<string> out{};

    string currentString;

    for (const auto& token : command) {
        if (!isspace(token)) {
            currentString += token;
        }
        else {
            if (!currentString.empty()) {
                out.push_back(currentString);
            }
            currentString = "";
        }
    }

    out.push_back(currentString);

    return out;
}

