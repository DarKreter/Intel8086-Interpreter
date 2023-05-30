#include "function.hpp"
#include <bitset>

using namespace std;

#define TEXT_START_BYTE 32

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
    size_t textSegmentSize = (fileContent[9] << 8) + fileContent[8];

    Analyze(fileContent + TEXT_START_BYTE, textSegmentSize);

    free(fileContent);

    return 0;
}