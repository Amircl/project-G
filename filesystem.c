#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "filesystem.h"

FILE* open_or_create_filesystem(const char* filename) {
    // Пытаемся открыть существующий файл для чтения/записи
    FILE* fs = fopen(filename, "r+");
    
    // Если файл не существует, создаём новый
    if (fs == NULL) {
        fs = fopen(filename, "w+");
        if (fs == NULL) {
            perror("Failed to create filesystem");
            return NULL;
        }
    }
    return fs;
}

char* view_file_in_filesystem(FILE* fs, const char* filename) {
    // Получаем размер файловой системы
    fseek(fs, 0, SEEK_END);
    long size = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    
    // Читаем всю файловую систему в буфер
    char* buffer = malloc(size + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    
    size_t bytes_read = fread(buffer, 1, size, fs);
    if (bytes_read != (size_t)size) {
        free(buffer);
        return NULL;
    }
    buffer[size] = '\0';
    
    // Ищем нужный файл
    char* file_start = strstr(buffer, filename);
    if (file_start == NULL) {
        free(buffer);
        return NULL;
    }
    
    // Находим начало содержимого файла
    char* content_start = strchr(file_start, '\n');
    if (content_start == NULL) {
        free(buffer);
        return NULL;
    }
    content_start++; // Пропускаем символ новой строки
    
    // Находим конец содержимого файла
    char* content_end = strchr(content_start, '/');
    if (content_end == NULL) {
        content_end = buffer + size;
    }
    
    // Копируем содержимое файла в отдельный буфер
    size_t content_length = content_end - content_start;
    char* content = malloc(content_length + 1);
    if (content == NULL) {
        free(buffer);
        perror("Memory allocation failed");
        return NULL;
    }
    
    strncpy(content, content_start, content_length);
    content[content_length] = '\0';
    
    free(buffer);
    return content;
}

int delete_file_in_filesystem(FILE* fs, const char* filename) {
    // Получаем текущую позицию в файле
    long original_pos = ftell(fs);
    
    // Читаем всю файловую систему
    fseek(fs, 0, SEEK_END);
    long size = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    
    char* buffer = malloc(size + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return -1;
    }
    
    fread(buffer, 1, size, fs);
    buffer[size] = '\0';
    
    // Ищем файл для удаления
    char* file_start = strstr(buffer, filename);
    if (file_start == NULL) {
        free(buffer);
        return -1;
    }
    
    // Находим конец файла (следующий '/' или конец буфера)
    char* file_end = strchr(file_start, '/');
    if (file_end == NULL) {
        file_end = buffer + size;
    } else {
        file_end++; // Включаем символ '/'
    }
    
    // Сдвигаем оставшуюся часть буфера
    size_t prefix_len = file_start - buffer;
    size_t suffix_len = (buffer + size) - file_end;
    
    memmove(file_start, file_end, suffix_len);
    
    // Перезаписываем файловую систему
    fseek(fs, 0, SEEK_SET);
    fwrite(buffer, 1, prefix_len + suffix_len, fs);
    ftruncate(fileno(fs), prefix_len + suffix_len);
    
    free(buffer);
    fseek(fs, original_pos, SEEK_SET);
    return 0;
}

int create_new_file(FILE* fs, const char* filename, const char* content) {
    // Проверяем, не существует ли уже такой файл
    fseek(fs, 0, SEEK_SET);
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fs) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(buffer, filename) == 0) {
            return -1; // Файл уже существует
        }
    }
    
    // Добавляем новый файл в конец
    fseek(fs, 0, SEEK_END);
    if (fprintf(fs, "%s\n%s\n/\n", filename, content) < 0) {
        perror("Failed to write to filesystem");
        return -1;
    }
    
    return 0;
}

int modify_file_content(FILE* fs, const char* filename, const char* new_content) {
    // Сохраняем текущую позицию
    long original_pos = ftell(fs);
    
    // Читаем всю файловую систему
    fseek(fs, 0, SEEK_END);
    long size = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    
    char* buffer = malloc(size + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return -1;
    }
    
    fread(buffer, 1, size, fs);
    buffer[size] = '\0';
    
    // Ищем файл для модификации
    char* file_start = strstr(buffer, filename);
    if (file_start == NULL) {
        free(buffer);
        return -1;
    }
    
    // Находим начало и конец содержимого
    char* content_start = strchr(file_start, '\n');
    if (content_start == NULL) {
        free(buffer);
        return -1;
    }
    content_start++;
    
    char* content_end = strchr(content_start, '/');
    if (content_end == NULL) {
        content_end = buffer + size;
    }
    
    // Создаём новый буфер с изменённым содержимым
    size_t prefix_len = content_start - buffer;
    size_t suffix_len = (buffer + size) - content_end;
    size_t new_size = prefix_len + strlen(new_content) + suffix_len + 3;
    
    char* new_buffer = malloc(new_size);
    if (new_buffer == NULL) {
        free(buffer);
        perror("Memory allocation failed");
        return -1;
    }
    
    // Копируем часть до содержимого
    memcpy(new_buffer, buffer, prefix_len);
    
    // Добавляем новое содержимое
    strcpy(new_buffer + prefix_len, new_content);
    
    // Добавляем конец файла и оставшуюся часть
    strcat(new_buffer, "\n/\n");
    strcat(new_buffer, content_end);
    
    // Перезаписываем файловую систему
    fseek(fs, 0, SEEK_SET);
    fwrite(new_buffer, 1, strlen(new_buffer), fs);
    ftruncate(fileno(fs), strlen(new_buffer));
    
    free(buffer);
    free(new_buffer);
    fseek(fs, original_pos, SEEK_SET);
    return 0;
}
