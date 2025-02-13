#include "includes.h"

char path[4096] = {0};

void search(int verbose, const char *targetDir, const char *dir)
{
    DIR *dp = opendir(dir);
    if (!dp) {
        if (errno == EACCES && verbose) {
            fprintf(stderr, "Permission denied: %s\n", dir);
        }
        return;
    }

    struct dirent *entry;
    struct stat st;
    char full[4096];

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, targetDir) == 0) {
            snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
            closedir(dp);
            return;
        }

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full, sizeof(full), "%s/%s", dir, entry->d_name);
        if (stat(full, &st) == -1) {
            if (errno == EACCES && verbose) {
                fprintf(stderr, "Permission denied: %s\n", full);
            }
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
            search(verbose, targetDir, full);
            if (path[0] != '\0') {
                closedir(dp);
                return;
            }
        }
    }
    closedir(dp);
}

void drop(int verbose)
{
    if (path[0] != '\0') {
        if (chdir(path) == 0) {
            if (verbose) {
                printf("Changed directory to: %s\n", path);
            }
            char *shell = getenv("SHELL");
            if (shell) {
                execl(shell, shell, "-l", (char *)NULL);
            } else {
                execl("/bin/zsh", "zsh", "-l", (char *)NULL);
            }
            perror("execl failed");
        } else {
            perror("chdir failed");
        }
    } else {
        if (verbose) {
            printf("Directory not found!\n");
        } else {
            printf("Directory not found! Use -f for details.\n");
        }
    }
}
