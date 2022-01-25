/* ** note: this file should be included in only one source file ** */

#include <stdio.h>
#include <stdlib.h>
#include "../include/common.h"

char* str_load(const char* filename) {
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

char* str_prefix(char* str, const char* prefix, bool_t free_str) {
  char* new_str = (char*)malloc(strlen(str) + strlen(prefix) + 1);
  strcpy(new_str, prefix);
  strcat(new_str, str);
  if (free_str) free(str);
  return new_str;
}
