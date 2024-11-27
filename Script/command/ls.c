#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

void listFiles(const char *directory) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    // 构造搜索模式
    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*", directory);

    // 查找文件
    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror("FindFirstFile failed");
        return;
    }

    printf("Listing files in directory: %s\n", directory);
    printf("%-30s %-10s %-20s\n", "----------------------", "----------", "--------------------");
    printf("%-30s %-10s %-20s\n", "Name", "Size (bytes)", "Last Modified");
    printf("%-30s %-10s %-20s\n", "----------------------", "----------", "--------------------");

    do {
        // 排除当前目录和父目录
        if (strcmp(findFileData.cFileName, ".") != 0 && 
            strcmp(findFileData.cFileName, "..") != 0) {
            
            // 获取文件类型
            const char *fileType = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? "Directory" : "File";

            // 获取文件大小
            LARGE_INTEGER fileSize;
            fileSize.LowPart = findFileData.nFileSizeLow;
            fileSize.HighPart = findFileData.nFileSizeHigh;

            // 获取最后修改时间
            FILETIME ft = findFileData.ftLastWriteTime;
            SYSTEMTIME st;
            FileTimeToSystemTime(&ft, &st);
            char timeString[100];
            snprintf(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d", 
                     st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);

            // 打印文件信息
            printf("%-30s %-10lld %-20s\n", findFileData.cFileName, fileSize.QuadPart, timeString);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    // 关闭句柄
    FindClose(hFind);
}

int main(int argc, char *argv[]) {
    char currentDir[MAX_PATH];

    // 获取当前工作目录
    if (GetCurrentDirectoryA(MAX_PATH, currentDir) == 0) {
        perror("GetCurrentDirectory failed");
        return EXIT_FAILURE;
    }

    // 如果没有提供路径参数，使用当前工作目录
    const char *directory = (argc < 2) ? currentDir : argv[1];

    listFiles(directory);

    return EXIT_SUCCESS;
}