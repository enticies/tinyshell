#include <iostream>
#include "../headers/util.h"
#include <vector>
#include <cctype>
#include <csignal>
#include <cstring>
#include <sys/wait.h>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <limits.h>



using namespace std;
using namespace filesystem;

vector<string> parse_command(const string &command);
// void print_parsed_command(const vector<string> &command);
char ** convert_vector_to_array(vector<string> &parsed_command);
void change_directory(char ** path);

int main() {
    string input;


    while (!cin.eof() && input != "exit") {
        char current_working_directory[PATH_MAX];

        if (getcwd(current_working_directory, sizeof(current_working_directory)) == NULL) {
            perror("getcwd() error");
        }

        char previous_directory[PATH_MAX];

        cout << "[" << current_working_directory << "]" << endl;
        cout << "> ";

        getline(cin, input);

        vector<string> parsed_command = parse_command(input);
        char ** parsed_command_array = convert_vector_to_array(parsed_command);

        int pid = fork();

        if (pid < 0) {
            perror("Failed to fork the process.");
        }
        else if (pid == 0) {
            if (*parsed_command_array) {
                if (parsed_command[0] == "cd") {
                    change_directory(++parsed_command_array);
                }
                else {
                    int exec_result = execvp(*parsed_command_array, parsed_command_array);

                    if (exec_result == -1) {
                        perror("Failed to execute the process");
                    }
                }

            }
        }
        wait(nullptr);

        for (size_t i = 0; parsed_command_array[i] != nullptr; ++i) {
            free(parsed_command_array[i]);
        }
    }


    return 0;
}

void change_directory(char **p) {
    struct passwd *pw = getpwuid(getuid());

    const char *homedir = pw->pw_dir;

    cout << homedir << endl;

    if (*p == nullptr) {
        current_path(homedir);
    }
    else if (strcmp(*p, "-") == 0) {

    }
    else {
        current_path(*p);
    }
}

char ** convert_vector_to_array(vector<string> &parsed_command) {
    char ** out = new char * [parsed_command.size() + 1];

    for (size_t i = 0; i < parsed_command.size(); ++i) {
        out[i] = strdup(parsed_command[i].c_str());
    }

    out[parsed_command.size()] = nullptr;

    return out;
}

void print_parsed_command(const vector<string> &command) {
    string out;

    for (int i = 0; i < command.size() - 1; i++) {
        out += command[i] + " ";
    }

    out += command.back();

    cout << out << endl;
}

// refactor so that this returns an array
vector<string> parse_command(const string &command) {
    vector<string> out{};

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

