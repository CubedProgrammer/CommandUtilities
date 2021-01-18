#include<stdio.h>
#include<string.h>
#define SIZE 1000000
char cbuf[1000000];
int file_word_counter(const char *fn, const char *wrd)
{
	FILE *fhandle = fopen(fn, "r");
	size_t size = fread(cbuf, 1, SIZE, fhandle), wl = strlen(wrd);
	cbuf[size] = '\0';
	const char *res = strstr(cbuf, wrd);
	int cnt = 0;
	fclose(fhandle);
	while(res)
	{
		++cnt;
		res = strstr(res + wl, wrd);
	}
	return cnt;
}
