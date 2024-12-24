
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef HISTORY_H
#define HISTORY_H

void display_history(FILE *history_file);
void pastevents_purge(FILE **history_file_ptr);
void add_to_history(const char *command, FILE *history_file);

#endif

