#include <stdio.h>
#include <stdlib.h>

inline char* LoadString(const char* filename) {
    FILE* f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    const long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* str = (char*)malloc(size + 1);
    fread(str, size, sizeof(char), f);
    str[size] = 0;
    fclose(f);
    return str;
}

inline char* PrefixString(char* str, const char* prefix, bool freeStr = true) {
    char* newStr = (char*)malloc(strlen(str) + strlen(prefix) + 1);
    strcpy(newStr, prefix);
    strcat(newStr, str);
    if (freeStr) free(str);
    return newStr;
}
