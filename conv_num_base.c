#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const char num_base_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
void conv_num_base(const char str[], int from, int to, char cbuf[])
{
    long long num = strtoll(str, NULL, from);
    int nega = 0;
    if(num < 0)
        nega = num = -num;
    int tmp;
    size_t len = 0;
    while(num)
    {
        tmp = num % to;
        num /= to;
        cbuf[len] = num_base_digits[tmp];
        ++len;
    }
    if(nega)
        cbuf[len++] = '-';
    cbuf[len] = '\0';
    char tmpch;
    for(size_t i = 0; i < len / 2; i++)
    {
        tmpch = cbuf[i];
        cbuf[i] = cbuf[len - i - 1];
        cbuf[len - i - 1] = tmpch;
    }
}