#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"get_sub_dir.h"
static inline int has_file_extension(const char *fname, const char *ext)
{
	if(ext == NULL)
		return 1;
	else
	{
		size_t flen = strlen(fname), elen = strlen(ext);
		int status = 0;
		if(flen > elen)
		{
			if(fname[flen - elen - 1] == '.' && strcmp(ext, fname + flen - elen) == 0)
				status = 1;
		}
		return status;
	}
}
int cnt_single_file(const char *file, size_t cr)
{
	FILE *f = fopen(file, "r");
	int cnt = 0;
	char curr;
	int ne = 0;

	// read the file
	while(feof(f) == 0)
	{
		curr = fgetc(f);
		if(curr == '\n')
		{
			if(ne >= cr)
				cnt++;
			ne = 0;
		}
		else if(curr != -1 && curr != ' ' && curr != '\t')
			++ne;
	}

	// if there is not a new line character at the end
	if(ne >= cr)
		++cnt;
	fclose(f);
	return cnt;
}
#ifdef _WIN32
long long
#else
long
#endif
csloc(const char *dir, size_t cr, int sif, int ihf, const char *const*fexts, size_t fel)
{
	// prepare to get the files and subdirectories
	char *subdir = malloc(100000);
	size_t len=strlen(dir), cnt=cnt_sub_dirs(dir);
	strcpy(subdir, dir);
#ifdef _WIN32
	subdir[len]='\\';
#else
	subdir[len]='/';
#endif
	char **names=malloc(sizeof(char*)*cnt);
	enum file_or_directory *tps = malloc(cnt * sizeof(enum file_or_directory));

	// get the files and subdirectories
	get_sub_dirs(dir, names, tps);

	// set up stack for simulating recursion
	size_t fcnt=0, rm=5, olr=3;
	char **stack=malloc(sizeof(char*)*rm);

	// source lines of code and single file lines
#ifndef _WIN32
	long sloc = 0;
#else
	long long sloc = 0;
#endif
	int sfl;
	int valid;

	// put all subdirectories in
	for(size_t i = 0; i < cnt; ++i)
	{
		strcpy(subdir + len + 1, names[i]);

		// get rid of hidden files if enabled
		if(ihf && names[i][0] == '.')
			continue;

		if(NFILE==tps[i])
		{
			valid = fel == 0 ? 1 : 0;
			for(size_t j = 0; j < fel; ++j)
			{
				if(has_file_extension(names[i],fexts[j]))
					valid++;
			}
			if(valid)
			{
				sfl = cnt_single_file(subdir, cr);
				if(sif)
					printf("File %s has %d source lines of code.\n", names[i], sfl);
				sloc += sfl;
			}
			continue;
		}

		// get rid of parent and self
		if(strcmp(".", names[i]) == 0 || strcmp("..", names[i]) == 0)
			continue;

		if(fcnt==rm)
		{
			stack=realloc(stack, (rm+olr)*sizeof(char*));
			fcnt=olr;
			olr=rm;
			rm+=fcnt;
			fcnt=olr;
		}

		stack[fcnt]=malloc(len + strlen(names[i]) + 2);
		strcpy(stack[fcnt], subdir);
		fcnt++;
		free(names[i]);
	}

	free(names);
	free(tps);

	// simulate recursion
	char *currf;
	while(fcnt)
	{
		// get last file
		currf = stack[--fcnt];
		len=strlen(currf), cnt=cnt_sub_dirs(currf);
		strcpy(subdir, currf);
#ifdef _WIN32
		subdir[len]='\\';
#else
		subdir[len]='/';
#endif

		// get subdirectories and files
		names=malloc(sizeof(char*)*cnt);
		tps = malloc(cnt * sizeof(enum file_or_directory));
		get_sub_dirs(currf, names, tps);

		// put all subdirectories in
		for(size_t i = 0; i < cnt; ++i)
		{
			strcpy(subdir + len + 1, names[i]);

			// get rid of hidden files if enabled
			if(ihf && names[i][0] == '.')
				continue;

			if(NFILE==tps[i])
			{
				valid = fel == 0 ? 1 : 0;
				for(size_t j = 0; j < fel; ++j)
				{
					if(has_file_extension(names[i],fexts[j]))
						valid++;
				}
				if(valid)
				{
					sfl = cnt_single_file(subdir, cr);
					if(sif)
						printf("File %s has %d source lines of code.\n", names[i], sfl);
					sloc += sfl;
				}
				continue;
			}

			// get rid of parent and self
			if(strcmp(".", names[i]) == 0 || strcmp("..", names[i]) == 0)
				continue;

			if(fcnt==rm)
			{
				stack=realloc(stack, (rm+olr)*sizeof(char*));
				fcnt=olr;
				olr=rm;
				rm+=fcnt;
				fcnt=olr;
			}

			stack[fcnt]=malloc(len + strlen(names[i]) + 2);
			strcpy(stack[fcnt], subdir);
			fcnt++;
			free(names[i]);
		}

		free(names);
		free(tps);
		free(currf);
	}
	free(stack);
	free(subdir);
	return sloc;
}
