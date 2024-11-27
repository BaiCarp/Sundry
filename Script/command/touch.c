#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <sys/stat.h>

void update_file_timestamp(const char *filename) {
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        // 如果文件不存在，创建一个新文件
        hFile = CreateFileA(
            filename,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if (hFile == INVALID_HANDLE_VALUE) {
            fprintf(stderr, "Error creating file: %s\n", filename);
            return;
        } else {
            // 文件创建成功，关闭句柄
            CloseHandle(hFile);
            return;
        }
    } else {
        // 文件存在，更新文件的最后写入时间
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        SetFileTime(hFile, &ft, NULL, &ft);
        CloseHandle(hFile);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        update_file_timestamp(argv[i]);
    }

    return EXIT_SUCCESS;
}