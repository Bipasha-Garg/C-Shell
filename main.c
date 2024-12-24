#include "headers.h"
#include <stdlib.h>
#include "warp.h"
#include "seek.h"
#include <string.h>

#ifndef __INPUT_H
#define __INPUT_H

void input_proc(char **list, int tokencount, char *home, char *prev_dir, int home_size, FILE *history_file);

#endif

char home[4000];
int home_size;

void trim_newline(char *str)
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

void home_dir()
{
    if (getcwd(home, sizeof(home)) == NULL)
    {
        perror("ERROR IN GETTING HOME DIRECTORY IN MAIN");
        exit(EXIT_FAILURE);
    }
    home_size = strlen(home);
}

FILE *open_or_create_file(const char *filename)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        perror("ERROR IN OPENING FILE IN MAIN");
        exit(EXIT_FAILURE);
    }
    return file;
}

int main()
{
    home_dir();
    FILE *history_file = open_or_create_file(HISTORY_FILE);
    const char *delimiters = ";";
    int tokencount = 0;
    char **list = NULL;
    char prev_dir[100];
    strcpy(prev_dir, home);

    while (1)
    {
        prompt(home, home_size);

        char input[4096];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            perror("ERROR IN GETTING INPUT FROM USER IN MAIN");
            exit(EXIT_FAILURE);
        }
        trim_newline(input);
        char *token = strtok(input, delimiters);

        while (token != NULL)
        {
            list = realloc(list, (tokencount + 1) * sizeof(char *));
            if (list == NULL)
            {
                perror("ERROR IN REALLOC OF **LIST IN MAIN");
                exit(EXIT_FAILURE);
            }
            list[tokencount] = malloc(strlen(token) + 1);
            if (list[tokencount] == NULL)
            {
                perror("ERROR IN MALLOC OF **LIST IN MAIN");
                exit(EXIT_FAILURE);
            }
            strcpy(list[tokencount], token);

            tokencount++;
            token = strtok(NULL, delimiters);
        }
        input_proc(list, tokencount, home, prev_dir, home_size, history_file);
        // for (int i = 0; i < tokencount; i++)
        // {
        //     free(list[i]);
        // }
        // free(list);
        // list = NULL;
        tokencount = 0;
    }
    if (fclose(history_file) != 0)
    {
        perror("ERROR IN CLOSE HISTORY FILE IN MAIN");
    }

    return 0;
}
