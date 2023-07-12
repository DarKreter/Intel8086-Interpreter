#include "binary.hpp"
#include "function.hpp"
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char* argv[], char** envp)
{
    enum class Mode_e {
        int_nolog = 0,
        int_log,
        diss
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
    std::vector<std::string> _argv;
    std::vector<std::string> _envp;

    _argv.push_back(argv[count]);
    uint8_t* fileContent = ReadFile(argv[count++]);

    for(; count < argc; count++)
        _argv.push_back(argv[count]);
    for(char** i = envp; *i != 0; i++)
        _envp.push_back(*i);

    _envp.clear();
    _envp.push_back("PATH=/usr:/usr/bin");

    Binary_t binary(fileContent);
    binary.StackInit(_argv, _envp);
    free(fileContent);

    if(mode == Mode_e::diss)
        Analyze(binary);
    else if(mode == Mode_e::int_log)
        Execute(binary, (bool)mode);
    else if(mode == Mode_e::int_nolog)
        Execute(binary);
    else {
        std::cout << "Unkown error!" << std::endl;
        exit(2);
    }

    return 0;
}