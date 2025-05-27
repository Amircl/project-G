#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

FILE* open_or_create_filesystem(const char* filename);
char* view_file_in_filesystem(FILE* fs, const char* filename);
int delete_file_in_filesystem(FILE* fs, const char* filename);
int create_new_file(FILE* fs, const char* filename, const char* content);
int modify_file_content(FILE* fs, const char* filename, const char* new_content);

#endif
