#ifdef _WIN32
#include<windows.h>
#else
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#endif
void file_redirect(const char *cmd, const char *in, const char *out, const char *err)
{
    int pid = fork();
    if(pid == 0)
    {
        int p_in, p_out, p_err;
        if(in != NULL)
        {
            p_in = open(in, O_RDONLY);
            dup2(p_in, STDIN_FILENO);
        }
        if(out != NULL)
        {
            p_out = open(out, O_WRONLY);
            dup2(p_out, STDOUT_FILENO);
        }
        if(err != NULL)
        {
            p_err = open(err, O_WRONLY);
            dup2(p_err, STDERR_FILENO);
        }
        exit(system(cmd));
    }
}