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

void fork_and_exec_with_io(char* cmd, int stdin_fd, int stdout_fd);

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

