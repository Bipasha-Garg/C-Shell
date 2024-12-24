
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#ifndef SEEK_H
#define SEEK_H

void call_seek(char *input);
void particular(char *target_dir, char *name, int flags);

#endif