#include "binary.hpp"

Binary_t::Binary_t(uint8_t* fileContent) : textPos{0}
{

    textSegmentSize = (fileContent[11] << 24) + (fileContent[10] << 16) +
                      (fileContent[9] << 8) + fileContent[8];
    dataSegmentSize = (fileContent[15] << 24) + (fileContent[14] << 16) +
                      (fileContent[13] << 8) + fileContent[12];

    text = new uint8_t[textSegmentSize];
    for(size_t i = 0; i < textSegmentSize; i++)
        text[i] = fileContent[i + TEXT_START_BYTE];

    data = new uint8_t[dataSegmentSize];
    for(size_t i = 0; i < dataSegmentSize; i++)
        data[i] = fileContent[i + TEXT_START_BYTE + textSegmentSize];
}

Binary_t::~Binary_t()
{
    text -= textPos;
    delete[] text;
    delete[] data;
}