#include<stdio.h>
int readbytes(const char *fname)
{
    // 50 bytes per line
    char unsigned cbuf[50];
    int rd = 0, tmp;
    FILE *fh = fopen(fname, "rb");
    tmp = fread(cbuf, 1, 50, fh);
    while(tmp)
    {
        for(int j = 0; j < tmp; j++)
        {
            if(cbuf[j] < 16)
                printf("0%x", cbuf[j]);
            else
                printf("%x", cbuf[j]);
        }
        fputc('\n', stdout);
        rd += tmp;
        tmp = fread(cbuf, 1, 50, fh);
    }
    fflush(stdout);
    fclose(fh);
    return rd;
}
