#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static char cbuf[1000000];
void file_word_replace(const char *fn, const char *wrd, const char *rep)
{
    FILE *fhandle = fopen(fn, "r");
    size_t cl = fread(cbuf, 1, sizeof(cbuf), fhandle);
    char *fc = malloc(sizeof(cbuf));
    char *tmp;
    for(size_t i = 0; i < cl; i++)
        fc[i] = cbuf[i];
    size_t rm = sizeof(cbuf);
    while(cl == sizeof(cbuf))
    {
        cl = fread(cbuf, 1, sizeof(cbuf), fhandle);
        tmp = malloc(rm + sizeof(cbuf));
        for(size_t i = 0; i < rm; i++)
            tmp[i] = fc[i];
        for(size_t i = 0; i < cl; i++)
            tmp[rm + i] = cbuf[i];
        rm += sizeof(cbuf);
        free(fc);
        fc = tmp;
    }
    size_t sz = rm - sizeof(cbuf) + cl;
    fc[sz] = '\0';
    fclose(fhandle);
    sz++;
    char *ns = malloc(sizeof(char));
    *ns = '\0';
    const char *res = strstr(fc, wrd);
    size_t wl = strlen(wrd), rl = strlen(rep), nl, ind = 0;
    rm = 0;
	while(res)
	{
	    nl = res - fc - ind;
	    tmp = malloc(rm + nl + rl + 1);
	    strcpy(tmp, ns);
        memcpy(tmp + rm, fc + ind, nl);
        strcpy(tmp + rm + nl, rep);
        free(ns);
        ns = tmp;
        rm += nl + rl;
        ind += nl + wl;
		tmp = strstr(res + wl, wrd);
		res = tmp;
	}
	if(fc[ind])
	{
	    ns = realloc(ns, rm + sz - ind);
	    strcpy(ns + rm, fc + ind);
    }
    fhandle = fopen(fn, "w");
    fputs(ns, fhandle);
    fclose(fhandle);
    free(fc);
    free(ns);
}
