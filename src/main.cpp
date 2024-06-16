#include <iostream>
#include "../headers/util.h"
#include <vector>
#include <cctype>
#include <csignal>
#include <cstring>

using namespace std;

vector<const char *> parse_command(const string & command);
// void print_parsed_command(const vector<string> &command);

int main() {
    string input;

    while (!cin.eof() && input != "exit") {
        cout << "> ";

        getline(cin, input);

        vector<const char *> parsed_command = parse_command(input);

        char ** pointers = new char * [parsed_command.size()];

        for (size_t i = 0; i < parsed_command.size(); ++i) {
            pointers[i] = new char[strlen(parsed_command[i]) + 1];
            strcpy(pointers[i], parsed_command[i]);
        }

        if (!parsed_command.empty()) {
            execvp(pointers[0], pointers);
            perror("execvp failed");
        }

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

vector<const char *> parse_command(const string &command) {
    vector<const char *> out{};

    string currentString;

    for (const auto& token : command) {
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

