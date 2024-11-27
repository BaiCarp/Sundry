#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void print_tree(const char *base, const char *path, int depth) {
    struct dirent *entry;
    DIR *dp = opendir(path);
    if (!dp) return;

    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        // Print the path for each item
        for (int i = 0; i < depth; i++) printf("©¦   ");
        struct stat statbuf;
        char new_path[1024];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
        if (stat(new_path, &statbuf) == -1) continue;
        if (S_ISDIR(statbuf.st_mode)) {
            printf("©À©¤©¤ [Folder] %s\n", entry->d_name);
            print_tree(base, new_path, depth + 1);
        } else {
            printf("©À©¤©¤ [File] %s\n", entry->d_name);
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {
    const char *path = (argc > 1) ? argv[1] : ".";
    printf("Path: %s\n", path);
    printf("-----------------------------------------------------------------------------\n");
    print_tree(path, path, 0);
    return 0;
}