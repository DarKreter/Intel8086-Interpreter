#include <iostream>
#include <stdio.h>
#include <stdlib.h>

unsigned char* ReadFile(char*);
bool CheckPattern(uint8_t*, size_t, std::string_view);
void Analyze(uint8_t*, size_t);
