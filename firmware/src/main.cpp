#include "binary.hpp"
#include "function.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2) {
        printf("No command-line arguments!\n");
        exit(1);
    }
    else if(argc > 2) {
        printf("Too many command-line arguments!\n");
        exit(2);
    }

    uint8_t* fileContent = ReadFile(argv[1]);
    Binary_t binary(fileContent);
    free(fileContent);

    // Analyze(binary);
    Execute(binary);

    return 0;
}