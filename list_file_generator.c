#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void list_file_generator(int lns, const char *fn)
{
    FILE *fhandle = strcmp(fn, "stdout") ? fopen(fn, "w") : stdout;
    for(int i = 0; i < lns; i++)
        fprintf(fhandle, "%d. \n", i + 1);
    if(strcmp(fn, "stdout"))
        fclose(fhandle);
}
