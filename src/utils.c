#include "includes.h"

FILE *fp = NULL;
const char *CDIR = "/.cache/vdrop";
const char *FI = "/path.txt";

int checkpl(char *line, char *target)
{
    int len = strlen(target);
    int line_len = strlen(line);

    if (line_len < len) {
        return 0;
    }

    if (strcmp(line + line_len - len, target) == 0) {
        if (line_len == len || line[line_len - len - 1] == '/') {
            return 1;
        }
    }
    return 0;
}

int checkfp()
{
    const char *home_env = getenv("HOME");
    if (!home_env) {
        fprintf(stderr, "HOME not set.\n");
        exit(EXIT_FAILURE);
    }

    char home[4096];
    strncpy(home, home_env, sizeof(home)-1);
    home[sizeof(home)-1] = '\0';

    char dirPath[4096];
    snprintf(dirPath, sizeof(dirPath), "%s%s", home, CDIR);

    struct stat sb;
    if (stat(dirPath, &sb) != 0 || !S_ISDIR(sb.st_mode)) {
        if (mkdir(dirPath, S_IRWXU) == -1) {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
    }

    char filePath[4096];
    snprintf(filePath, sizeof(filePath), "%s%s", dirPath, FI);

    if (access(filePath, F_OK) != 0) {
        fp = fopen(filePath, "a");
        if (!fp) {
            fprintf(stderr, "Failed to create file: %s\n", filePath);
            exit(EXIT_FAILURE);
        }
        fclose(fp);
    }

    return 0;
}
