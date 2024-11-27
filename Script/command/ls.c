#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

void listFiles(const char *directory) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    // ��������ģʽ
    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*", directory);

    // �����ļ�
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
        // �ų���ǰĿ¼�͸�Ŀ¼
        if (strcmp(findFileData.cFileName, ".") != 0 && 
            strcmp(findFileData.cFileName, "..") != 0) {
            
            // ��ȡ�ļ�����
            const char *fileType = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? "Directory" : "File";

            // ��ȡ�ļ���С
            LARGE_INTEGER fileSize;
            fileSize.LowPart = findFileData.nFileSizeLow;
            fileSize.HighPart = findFileData.nFileSizeHigh;

            // ��ȡ����޸�ʱ��
            FILETIME ft = findFileData.ftLastWriteTime;
            SYSTEMTIME st;
            FileTimeToSystemTime(&ft, &st);
            char timeString[100];
            snprintf(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d", 
                     st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);

            // ��ӡ�ļ���Ϣ
            printf("%-30s %-10lld %-20s\n", findFileData.cFileName, fileSize.QuadPart, timeString);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    // �رվ��
    FindClose(hFind);
}

int main(int argc, char *argv[]) {
    char currentDir[MAX_PATH];

    // ��ȡ��ǰ����Ŀ¼
    if (GetCurrentDirectoryA(MAX_PATH, currentDir) == 0) {
        perror("GetCurrentDirectory failed");
        return EXIT_FAILURE;
    }

    // ���û���ṩ·��������ʹ�õ�ǰ����Ŀ¼
    const char *directory = (argc < 2) ? currentDir : argv[1];

    listFiles(directory);

    return EXIT_SUCCESS;
}