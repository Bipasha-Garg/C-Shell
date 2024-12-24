#include "headers.h"
#include "bg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
int numBackgroundProcesses = 0;

void executeCommand(char *command)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("FORK ERROR IN BG.C");
        return;
    }

    if (pid == 0)
    {
        char *args[1024];
        char *token;
        int argCount = 0;

        token = strtok(command, " ");
        while (token != NULL)
        {
            args[argCount] = token;
            argCount++;
            token = strtok(NULL, " ");
        }
        args[argCount] = NULL;

        if (execvp(args[0], args) == -1)
        {
            perror("ERROR IN EXECVP IN BG.C");
            exit(1);
        }
    }
    else
    {
        if (numBackgroundProcesses < MAX_BACKGROUND_PROCESSES)
        {
            strncpy(backgroundProcesses[numBackgroundProcesses].command, command, sizeof(backgroundProcesses[numBackgroundProcesses].command));
            backgroundProcesses[numBackgroundProcesses].pid = pid;
            numBackgroundProcesses++;
            printf("%d\n", pid);
        }
        else
        {
            printf("MAX BG PROCESS.\n");
        }
    }
}

void checkBackgroundProcesses()
{
    int i;
    int status;

    for (i = 0; i < numBackgroundProcesses; i++)
    {
        pid_t result = waitpid(backgroundProcesses[i].pid, &status, WNOHANG);

        if (result == -1)
        {
            perror("ERROR IN WAITPID IN BG.C");
            continue;
        }
        else if (result == 0)
        {
            continue;
        }
        else
        {
            if (WIFEXITED(status))
            {
                backgroundProcesses[i].status = WEXITSTATUS(status);
                printf("%s exited normally with status %d\t(%d)\n", backgroundProcesses[i].command, backgroundProcesses[i].status, backgroundProcesses[i].pid);
            }
            else if (WIFSIGNALED(status))
            {
                backgroundProcesses[i].status = -1;
                printf("%s exited due to a signal %d\t(%d)\n", backgroundProcesses[i].command, WTERMSIG(status), backgroundProcesses[i].pid);
            }
            else
            {
                backgroundProcesses[i].status = -1;
                printf("%s exited abnormally\t (%d)\n", backgroundProcesses[i].command, backgroundProcesses[i].pid);
            }

            for (int j = i; j < numBackgroundProcesses - 1; j++)
            {
                backgroundProcesses[j] = backgroundProcesses[j + 1];
            }

            numBackgroundProcesses--;
            i--;
        }
    }
}
