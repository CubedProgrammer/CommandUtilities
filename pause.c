#ifdef __linux__
#include<stdio.h>
#endif
#include<stdlib.h>
#ifdef __linux__
#include<termios.h>
#include<unistd.h>
#endif
void console_pause(void)
{
#ifdef _WIN32
    system("pause");
#else
    struct termios tdat;
    tcgetattr(STDIN_FILENO, &tdat);
    tdat.c_lflag &= ~ECHO;
    tdat.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &tdat);
    fputs("Press any key to continue.", stdout);
    getchar();
    tdat.c_lflag |= ECHO;
    tdat.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &tdat);
    fputc('\n', stdout);
#endif
}