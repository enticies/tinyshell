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

void print_parsed_command(const vector<string> &command);

char **convert_vector_to_array(vector<string> &parsed_command);

void change_directory(char **path);

void fork_and_exec_with_io(char *cmd, int stdout_fd, int stdin_fd);

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
        char **parsed_command_array = convert_vector_to_array(parsed_command);

        if (parsed_command.empty()) {
            continue;
        }

        if (parsed_command[0] == "cd") {
            change_directory(++parsed_command_array);
        }
        else if (parsed_command.size() == 3 && parsed_command[1] == "|") {
            fork_and_exec_with_io(parsed_command_array[0], 0, 1);
        }
        else {
            int pid = fork();
            

            if (pid < 0) {
                perror("Failed to fork");
            }
            else if (pid == 0) {
                int exec_result = execvp(*parsed_command_array, parsed_command_array);

                if (exec_result == -1) {
                    throw std::runtime_error("Failed to execute execvp");
                }
            }
            wait(nullptr);
        }


        for (size_t i = 0; parsed_command_array[i] != nullptr; ++i) {
            free(parsed_command_array[i]);
        }
    }


    return 0;
}

void fork_and_exec_with_io(char *cmd, int stdin_fd, int stdout_fd) {
    int pid = fork();

    if (pid < 0) {
        throw std::runtime_error("Failed to fork");
    }

    int pipe_fds[2];

    pipe(pipe_fds);

    if (dup2(pipe_fds[0], stdin_fd) == -1 || dup2(pipe_fds[1], stdout_fd) == -1) {
        throw std::runtime_error("Failed to dup2");
    }

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

