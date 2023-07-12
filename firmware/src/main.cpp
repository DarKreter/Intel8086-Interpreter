#include "binary.hpp"
#include "function.hpp"
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    enum class Mode_e {
        diss,
        int_log,
        int_nolog
    } mode = Mode_e::int_nolog;
    uint8_t count = 1;

    if(argc < 2) {
        printf("No command-line arguments!\n");
        exit(1);
    }
    while(argv[count][0] == '-') {
        if(strcmp(argv[count], "-d") == 0)
            mode = Mode_e::diss;
        else if(strcmp(argv[count], "-m") == 0)
            mode = Mode_e::int_log;
        else {
            printf("Unkown flag: %s\n", argv[count]);
            exit(5);
        }
        count++;
    }

    uint8_t* fileContent = ReadFile(argv[count++]);
    Binary_t binary(fileContent);
    free(fileContent);

    if(mode == Mode_e::diss)
        Analyze(binary);
    else if(mode == Mode_e::int_log)
        Execute(binary);
    else if(mode == Mode_e::int_nolog)
        Execute(binary);
    else {
        std::cout << "Unkown error!" << std::endl;
        exit(2);
    }

    return 0;
}