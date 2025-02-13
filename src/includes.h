#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>

extern const char *VERSION;
extern volatile int done;
extern const char *frames[];
extern const int nframes;
#endif // INCLUDES_H_
