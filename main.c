#include <stdio.h>
#include <stdlib.h>
#include "filesystem.h"

int main() {
    const char* fs_name = "my_fs.disk";
    const char* test_file = "test.txt";
    const char* test_content = "Hello, File System!";
    
    // 1. Создаем/открываем файловую систему
    FILE* fs = open_or_create_filesystem(fs_name);
    if (!fs) {
        printf("Failed to open/create filesystem\n");
        return 1;
    }
    
    // 2. Создаем новый файл
    if (create_new_file(fs, test_file, test_content) {
        printf("Failed to create file\n");
        fclose(fs);
        return 1;
    }
    printf("File created successfully\n");
    
    // 3. Читаем содержимое файла
    char* content = view_file_in_filesystem(fs, test_file);
    if (content) {
        printf("File content: %s\n", content);
        free(content);
    } else {
        printf("Failed to read file\n");
    }
    
    fclose(fs);
    return 0;
}
