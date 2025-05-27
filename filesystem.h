#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

/* Открывает или создает файловую систему */
FILE* open_or_create_filesystem(const char* filename);

/* Просматривает содержимое указанного файла */
char* view_file_in_filesystem(FILE* fs, const char* filename);

/* Удаляет файл из файловой системы */
int delete_file_in_filesystem(FILE* fs, const char* filename);

/* Создает новый файл с указанным содержимым */
int create_new_file(FILE* fs, const char* filename, const char* content);

/* Изменяет содержимое существующего файла */
int modify_file_content(FILE* fs, const char* filename, const char* new_content);

#endif
