#ifndef FUNCTION_DIS
#define FUNCTION_DIS

#include "commands.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

unsigned char* ReadFile(char*);
bool CheckPattern(uint8_t*, size_t, std::string_view);
void Analyze(uint8_t*, size_t);

template <typename... Ts>
Command_t* CheckAllCommands(uint8_t* tab, size_t diff);
template <typename T>
bool CheckCommand(uint8_t* tab, size_t diff);

#endif // FUNCTION_DIS