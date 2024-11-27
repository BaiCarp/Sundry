#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

void cat(const char *filename) {
    FILE *file = fopen(filename, "rb"); // 以二进制模式打开文件
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // 设置为UTF-8编码
    setlocale(LC_ALL, ""); // 使用系统默认的区域设置
    _setmode(_fileno(stdout), _O_U8TEXT); // 设置标准输出为UTF-8模式

    char buffer[1024];
    while (fread(buffer, sizeof(char), sizeof(buffer), file) > 0) {
        // 将UTF-8字符转换为宽字符
        int wideCharCount = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, NULL, 0);
        if (wideCharCount > 0) {
            wchar_t *wideBuffer = malloc(wideCharCount * sizeof(wchar_t));
            if (wideBuffer != NULL) {
                MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wideBuffer, wideCharCount);
                wprintf(L"%ls", wideBuffer); // 输出宽字符
                free(wideBuffer);
            } else {
                perror("Memory allocation failed");
            }
        } else {
            perror("Error converting to wide character");
            break;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        cat(argv[i]);
    }

    return EXIT_SUCCESS;
}