#include <iostream>
#include "../headers/util.h"
#include <cstdlib>

using namespace std;

int main() {
    string input;

    while (input != "exit") {
        cout << "> ";

        getline(cin, input);
        trim(input);

        int result = system(input.c_str());
    }

    return 0;
}
