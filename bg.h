

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef __BG_H_
#define __BG_H_

#define MAX_BACKGROUND_PROCESSES 10
#define MAX_COMMAND_LENGTH 1024

struct BackgroundProcess
{
    pid_t pid;                       
    char command[MAX_COMMAND_LENGTH]; 
    int status;                       
};

extern struct BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES];
extern int numBackgroundProcesses;

void executeCommand(char *command);
void checkBackgroundProcesses();

#endif

