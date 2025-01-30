#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>

void help()
{
    printf("Usage: vdrop [options] <directory>\n");
    printf("Options:\n");
    printf("  -f          Show detailed output.\n");
    printf("  -h, --help  Show this help.\n");
    printf("Example: vdrop Documents\n");
    exit(0);
}

char path[4096] = {0};  /* directory path */
int done = 0;           /* search done flag */

/* animation frames */
const char *frames[] = {
    "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"
};
const int nframes = sizeof(frames) / sizeof(frames[0]);

void *loading(void *arg)
{
    while (!done) {
        for (int i = 0; i < nframes && !done; i++) {
            printf("\rSearching... %s ", frames[i]);
            fflush(stdout);
            usleep(100000);
        }
    }
    printf("\r\033[K");
    fflush(stdout);

    return NULL;
}

/* search for directory */
void search(int verbose, const char *directory, const char *dir)
{
    struct dirent *entry;
    struct stat st;
    char full[4096];

    DIR *dp = opendir(dir);
    if (!dp) {
        if (errno == EACCES && verbose) {
            fprintf(stderr, "Permission denied: %s\n", dir);
        }
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, directory) == 0) {
            snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
            closedir(dp);
            return;
        }

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full, sizeof(full), "%s/%s", dir, entry->d_name);
        if (stat(full, &st) == -1) {
            if (errno == EACCES && verbose) {
                fprintf(stderr, "Permission denied: %s\n", full);
            }
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            search(verbose, directory, full);
            if (path[0] != '\0') {
                closedir(dp);
                return;
            }
        }
    }


    closedir(dp);
}

/* switch to directory */
void drop(int verbose)
{
    if (path[0] != '\0') {
        if (chdir(path) == 0) {
            if (verbose) {
                printf("Changed to: %s\n", path);
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
        if (!verbose) {
            printf("\rDirectory not found! Use -f for details.\n");
        } else {
            printf("Directory not found!\n");
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        help();
        return 1;
    }


    if (argv[1][0] == '-') {
        if (strcmp(argv[1], "-f") != 0 &&
            strcmp(argv[1], "-h") != 0 &&
            strcmp(argv[1], "--help") != 0) {
            fprintf(stderr, "Invalid option: %s\n", argv[1]);
            fprintf(stderr, "Use -h or --help for more information.\n");
            return 1;
        }

        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            help();
        }

        if (strcmp(argv[1], "-f") == 0 && argc < 3) {
            fprintf(stderr, "Error: -f option requires a directory name.\n");
            fprintf(stderr, "Use -h or --help for more information.\n");
            return 1;
        }
    }

    char cwd[4096];
    if (!getcwd(cwd, sizeof(cwd))) {
        perror("getcwd");
        return 1;
    }

    int verbose = 0;
    if (argc >= 3 && strcmp(argv[1], "-f") == 0) {
        verbose = 1;
    }

    pthread_t anim;
    pthread_create(&anim, NULL, loading, NULL);

    search(verbose, argv[argc - 1], cwd);

    done = 1;
    pthread_join(anim, NULL);

    drop(verbose);

    return 0;
} 
