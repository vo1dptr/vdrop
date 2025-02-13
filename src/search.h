#ifndef SEARCH_H_
#define SEARCH_H_

extern char path[4096];

void search(int verbose, const char *targetDir, const char *dir);
void drop(int verbose);

#endif // SEARCH_H_
