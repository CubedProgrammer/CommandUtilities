#include<stdlib.h>
#include<string.h>
#ifdef __linux__
// commands on linux
static const char mvc[] = "mv ";
static const char rmc[] = "rm ";
static const char rmd[] = "rm -r ";
#elif defined _WIN32
// commands on windows
static const char mvc[] = "move /y ";
static const char rmc[] = "del ";
static const char rmd[] = "rmdir /s /q ";
#endif

int rmdr(const char* dir)
{
    size_t rlen = strlen(rmd), dlen = strlen(dir);
    size_t clen = rlen + dlen + 1;
    char* cmd = malloc(clen * sizeof(char));

    strcpy(cmd, rmd);
    strcpy(cmd + rlen, dir);
    cmd[clen - 1] = '\0';

    int r = system(cmd);
    free(cmd);
    return r;
}

int del(const char* file)
{
    size_t rlen = strlen(rmc), flen = strlen(file);
    size_t clen = rlen + flen + 1;
    char* cmd = malloc(clen * sizeof(char));

    strcpy(cmd, rmc);
    strcpy(cmd + rlen, file);
    cmd[clen - 1] = '\0';

    int r = system(cmd);
    free(cmd);
    return r;
}

int move(const char* from, const char* to)
{
    size_t mlen = strlen(mvc), flen = strlen(from), tlen = strlen(to);
    size_t clen = mlen + flen + tlen + 2;
    char* cmd = malloc(clen * sizeof(char));

    strcpy(cmd, mvc);
    strcpy(cmd + mlen, from);
    cmd[mlen + flen] = ' ';
    strcpy(cmd + mlen + flen + 1, to);
    cmd[clen - 1] = '\0';
    
    int r = system(cmd);
    free(cmd);
    return r;
}