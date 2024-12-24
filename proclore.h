#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#ifndef __PROCLORE_H
#define __PROCLORE_H
void proclore_func(char *home,int home_size,char *extract);
void proc_details(char *extract, char *home,pid_t pid);
#endif