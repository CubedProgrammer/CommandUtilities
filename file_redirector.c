#ifdef _WIN32
#include<windows.h>
#else
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#endif
void file_redirect(const char *cmd, const char *in, const char *out, const char *err)
{
#ifndef _WIN32
    int pid = fork();
    if(pid == 0)
    {
#endif
#ifdef _WIN32
        SECURITY_ATTRIBUTES attr;
        attr.nLength = sizeof(attr);
        attr.bInheritHandle = TRUE;
        attr.lpSecurityDescriptor = NULL;
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));
        si.cb = sizeof(si);
        OFSTRUCT ofs;
        HANDLE
#else
        int
#endif
        p_in, p_out, p_err;
        if(in != NULL)
        {
#ifdef _WIN32
            si.hStdInput = (HANDLE)OpenFile(in, &ofs, OF_READ);
#else
            p_in = open(in, O_RDONLY);
            dup2(p_in, STDIN_FILENO);
#endif
        }
        if(out != NULL)
        {
#ifdef _WIN32
            si.hStdOutput = (HANDLE)OpenFile(out, &ofs, OF_WRITE);
            /*CreatePipe(&p_out, &si.hStdOutput, &attr, 0);
            SetHandleInformation(p_out, HANDLE_FLAG_INHERIT, 0);*/
#else
            p_out = open(out, O_WRONLY);
            dup2(p_out, STDOUT_FILENO);
#endif
        }
        if(err != NULL)
        {
#ifdef _WIN32
            si.hStdError = (HANDLE)OpenFile(err, &ofs, OF_WRITE);
            /*CreatePipe(&p_err, &si.hStdError, &attr, 0);
            SetHandleInformation(p_err, HANDLE_FLAG_INHERIT, 0);*/
#else
            p_err = open(err, O_WRONLY);
            dup2(p_err, STDERR_FILENO);
#endif
        }
        si.dwFlags |= STARTF_USESTDHANDLES;
        char cmdarg[MAX_PATH];
        strcpy(cmdarg, cmd);
        CreateProcessA(NULL, cmdarg, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
#ifndef _WIN32
        exit(system(cmd));
    }
#endif
}