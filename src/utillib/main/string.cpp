#include "string.h"

bool stringBeginsWith(const char* input, const char* match)
{
    return strncmp(match, input, strlen(match)) == 0;
}

bool stringEndsWith(const char* input, const char* match)
{
    auto inputLength = strlen(input);
    auto matchLength = strlen(match);
    return inputLength >= matchLength
        && strcmp(input + inputLength - matchLength, match) == 0;
}

