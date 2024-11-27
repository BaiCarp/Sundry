#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void backup_file(const char *filename) {
    char backup_filename[MAX_PATH];
    snprintf(backup_filename, sizeof(backup_filename), "%s.bak", filename);
    if (CopyFile(filename, backup_filename, FALSE)) {
        printf("Backup created: %s\n", backup_filename);
    } else {
        printf("Failed to create backup for %s. Error: %lu\n", filename, GetLastError());
    }
}

int file_exists(const char *filename) {
    return GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES;
}

int is_file_newer(const char *source, const char *destination) {
    WIN32_FILE_ATTRIBUTE_DATA source_info, dest_info;
    GetFileAttributesEx(source, GetFileExInfoStandard, &source_info);
    GetFileAttributesEx(destination, GetFileExInfoStandard, &dest_info);
    return CompareFileTime(&source_info.ftLastWriteTime, &dest_info.ftLastWriteTime) > 0;
}

void move_file(const char *source, const char *destination, int backup, int interactive, int force, int no_overwrite, int update, int verbose) {
    if (file_exists(destination)) {
        if (no_overwrite) {
            if (verbose) {
                printf("Skipping: %s already exists.\n", destination);
            }
            return;
        }
        if (interactive) {
            char response;
            printf("File %s already exists. Overwrite? (y/n): ", destination);
            response = getchar();
            getchar(); // Consume newline character
            if (response != 'y') {
                printf("Operation cancelled.\n");
                return;
            }
        } else if (!force) {
            if (backup) {
                backup_file(destination);
            }
        }
    }

    if (update && file_exists(destination) && !is_file_newer(source, destination)) {
        if (verbose) {
            printf("Skipping: %s is not newer than %s.\n", source, destination);
        }
        return;
    }

    if (MoveFile(source, destination)) {
        if (verbose) {
            printf("Moved: %s to %s\n", source, destination);
        }
    } else {
        printf("Failed to move file from %s to %s. Error: %lu\n", source, destination, GetLastError());
    }
}

int main(int argc, char *argv[]) {
    int backup = 0, interactive = 0, force = 0, no_overwrite = 0, update = 0, verbose = 0;
    char *source = NULL;
    char *destination = NULL;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-b") == 0) {
                backup = 1;
            } else if (strcmp(argv[i], "-i") == 0) {
                interactive = 1;
            } else if (strcmp(argv[i], "-f") == 0) {
                force = 1;
            } else if (strcmp(argv[i], "-n") == 0) {
                no_overwrite = 1;
            } else if (strcmp(argv[i], "-u") == 0) {
                update = 1;
            } else if (strcmp(argv[i], "-v") == 0) {
                verbose = 1;
            }
        } else {
            if (!source) {
                source = argv[i];
            } else {
                destination = argv[i];
            }
        }
    }

    if (!source || !destination) {
        printf("Usage: %s [options] <source_file> <destination_file>\n", argv[0]);
	printf("Options:\n");
    	printf("  -b, --backup       If the target file exists, create a backup before overwriting (with .bak suffix).\n");
    	printf("  -i, --interactive  If the target file exists, prompt the user for confirmation before overwriting.\n");
    	printf("  -f, --force        If the target file exists, overwrite without prompting.\n");
    	printf("  -n, --no-clobber   If the target file exists, skip the move operation.\n");
    	printf("  -u, --update       Only move the source file to the destination if the source is newer than the destination.\n");
    	printf("  -v, --verbose      Display detailed information about the operation.\n");
    	printf("  -h, --help         Display this help and exit.\n");
	
	return 1;
    }

    move_file(source, destination, backup, interactive, force, no_overwrite, update, verbose);
    return 0;
}