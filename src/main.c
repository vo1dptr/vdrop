#include "includes.h"
#include "search.h"
#include "utils.h"

void help(void)
{
    printf("Usage: vdrop [options] <directory>\n");
    printf("Options:\n");
    printf("  -f                   Show detailed output.\n");
    printf("  -a, --add PATH       Add a path manually to the cache.\n");
    printf("  -l, --list           List cached paths with line numbers.\n");
    printf("  -d, --delete NUMBER  Delete cached path with the specified line number.\n");
    printf("  -v, --version        vDrop Version.\n");
    printf("  -h, --help           Show this help.\n");
    printf("\nExamples:\n");
    printf("  vdrop -a /home/user/path\n");
    printf("  vdrop -l\n");
    printf("  vdrop -d 2\n");
    printf("  vdrop Documents\n");
    exit(0);
}

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

void lcache(void)
{
    const char *home_env = getenv("HOME");
    if (!home_env) {
        fprintf(stderr, "HOME not set.\n");
        return;
    }
    char home[4096];
    strncpy(home, home_env, sizeof(home) - 1);
    home[sizeof(home) - 1] = '\0';

    char cacheFile[4096];
    snprintf(cacheFile, sizeof(cacheFile), "%s%s%s", home, CDIR, FI);

    FILE *cfp = fopen(cacheFile, "r");
    if (!cfp) {
        fprintf(stderr, "Unable to open cache file: %s\n", cacheFile);
        return;
    }
    char line[4096];
    int line_no = 1;
    while (fgets(line, sizeof(line), cfp)) {
        line[strcspn(line, "\n")] = '\0';
        printf("%d: %s\n", line_no, line);
        line_no++;
    }
    fclose(cfp);
}

void delcache_line(int deline)
{
    const char *home_env = getenv("HOME");
    if (!home_env) {
        fprintf(stderr, "HOME not set.\n");
        return;
    }
    char home[4096];
    strncpy(home, home_env, sizeof(home) - 1);
    home[sizeof(home) - 1] = '\0';

    char cacheFile[4096];
    snprintf(cacheFile, sizeof(cacheFile), "%s%s%s", home, CDIR, FI);

    FILE *cfp = fopen(cacheFile, "r");
    if (!cfp) {
        fprintf(stderr, "Unable to open cache file: %s\n", cacheFile);
        return;
    }

    char *lines[1024];
    int count = 0;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), cfp) && count < 1024) {
        lines[count] = strdup(buffer);
        count++;
    }
    fclose(cfp);

    if (deline < 1 || deline > count) {
        fprintf(stderr, "Invalid line number\n");
        for (int i = 0; i < count; i++) {
            free(lines[i]);
        }
        return;
    }

    free(lines[deline - 1]);
    for (int i = deline - 1; i < count - 1; i++) {
        lines[i] = lines[i + 1];
    }
    count--;

    cfp = fopen(cacheFile, "w");
    if (!cfp) {
        fprintf(stderr, "Unable to open cache file for writing: %s\n", cacheFile);
        for (int i = 0; i < count; i++) {
            free(lines[i]);
        }
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(cfp, "%s", lines[i]);
        free(lines[i]);
    }
    fclose(cfp);
    printf("Deleted entry %d successfully.\n", deline);
}

void appendCache(const char *path)
{
    const char *home_env = getenv("HOME");
    if (!home_env) {
        fprintf(stderr, "HOME not set.\n");
        return;
    }
    char home[4096];
    strncpy(home, home_env, sizeof(home) - 1);
    home[sizeof(home) - 1] = '\0';
    char cacheFile[4096];
    snprintf(cacheFile, sizeof(cacheFile), "%s%s%s", home, CDIR, FI);
    FILE *fp = fopen(cacheFile, "a");
    if (!fp) {
        fprintf(stderr, "Unable to open cache file: %s\n", cacheFile);
        return;
    }
    fprintf(fp, "%s\n", path);
    fclose(fp);
}

int main(int argc, char **argv)
{
    int verbose = 0;

    if (argc < 2) {
        help();
    }

    if (argv[1][0] == '-') {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            help();
        }
        else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            printf("vDrop - v%s\n", VERSION);
            return 0;
        }
        else if (strcmp(argv[1], "-f") == 0) {
            verbose = 1;
            if (argc < 3) {
                fprintf(stderr, "Error: -f option requires a directory name.\n");
                fprintf(stderr, "Use -h or --help for more information.\n");
                return 1;
            }
        }
        else if ((strcmp(argv[1], "-a") == 0) || (strcmp(argv[1], "--add") == 0)) {
            if (argc < 3) {
                fprintf(stderr, "Error: -a/--add requires a path argument.\n");
                return 1;
            }
            checkfp();
            appendCache(argv[2]);
            printf("Added path to cache: %s\n", argv[2]);
            return 0;
        }
        else if ((strcmp(argv[1], "-l") == 0) || (strcmp(argv[1], "--list") == 0)) {
            checkfp();
            lcache();
            return 0;
        }
        else if ((strcmp(argv[1], "-d") == 0) || (strcmp(argv[1], "--delete") == 0)) {
            if (argc < 3) {
                fprintf(stderr, "Error: -d/--delete requires a line number argument.\n");
                return 1;
            }
            int deline = atoi(argv[2]);
            if (deline <= 0) {
                fprintf(stderr, "Error: Invalid line number.\n");
                return 1;
            }
            checkfp();
            delcache_line(deline);
            return 0;
        }
        else {
            fprintf(stderr, "Invalid option: %s\n", argv[1]);
            fprintf(stderr, "Use -h or --help for more information.\n");
            return 1;
        }
    }

    const char *targetDir = argv[argc - 1];
    checkfp();

    const char *homenv = getenv("HOME");
    if (!homenv) {
        fprintf(stderr, "HOME not set.\n");
        exit(EXIT_FAILURE);
    }
    char home[4096];
    strncpy(home, homenv, sizeof(home) - 1);
    home[sizeof(home) - 1] = '\0';
    char cacheFile[4096];
    snprintf(cacheFile, sizeof(cacheFile), "%s%s%s", home, CDIR, FI);

    FILE *cfp = fopen(cacheFile, "r");
    if (cfp) {
        char line[4096];
        while (fgets(line, sizeof(line), cfp)) {
            line[strcspn(line, "\n")] = '\0';
            if (checkpl(line, (char *)targetDir)) {
                strncpy(path, line, sizeof(path) - 1);
                break;
            }
        }
        fclose(cfp);
    }

    if (path[0] == '\0') {
        char cwd[4096];
        if (!getcwd(cwd, sizeof(cwd))) {
            perror("getcwd");
            return 1;
        }

        pthread_t anim;
        if (pthread_create(&anim, NULL, loading, NULL) != 0) {
            perror("pthread_create");
            return 1;
        }

        search(verbose, targetDir, cwd);

        done = 1;
        pthread_join(anim, NULL);

        if (path[0] != '\0') {
            appendCache(path);
        }
    }
    else {
        if (verbose) {
            printf("Found in cache: %s\n", path);
        }
    }

    drop(verbose);
    return 0;
}
