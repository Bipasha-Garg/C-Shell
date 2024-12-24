#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "input.h"
#include "warp.h"
#include "history.h"
#include "headers.h"
#include "proclore.h"
#include "seek.h"
#include "bg.h"

void sleep_command(char *home, char *prev_dir, int home_size, char *extract)
{
    int seconds = atoi(extract);

    if (seconds <= 0)
    {
        printf("Invalid sleep duration: %s\n", extract);
        return;
    }

    printf("Sleeping for %d seconds...\n", seconds);

    sleep(seconds);
    printf("wake up!\n");
    return;
}

void echo_command(char *home, char *prev_dir, int home_size, char *extract)
{
    printf("%s\n", extract);
    return;
}

void warp_command(char *home, char *prev_dir, int home_size, char *extract)
{
    warp_ok(home, prev_dir, home_size, extract);
}
void peek_command(char *home, char *prev_dir, int home_size, char *extract)
{
    peek(home, prev_dir, home_size, extract);
}
void proclore_command(char *home, char *prev_dir, int home_size, char *extract)
{
    proclore_func(home, home_size, extract);
}
void seek_command(char *home, char *prev_dir, int home_size, char *extract)
{
    call_seek(extract);
}

typedef void (*CommandFunction)(char *, char *, int, char *);

struct CommandTable
{
    const char *command;
    CommandFunction function;
} commands[] = {
    {"sleep", (CommandFunction)sleep_command},
    {"echo", (CommandFunction)echo_command},
    {"warp", (CommandFunction)warp_command},
    {"peek", (CommandFunction)peek_command},
    {"proclore", (CommandFunction)proclore_command},
    {"seek", (CommandFunction)seek_command}, // Add "seek" command here
    {NULL, NULL}};

void split_command(char *input, char **bg, char **fg)
{
    char *ampersand_pos = strchr(input, '&');

    if (ampersand_pos != NULL)
    {
        int bg_length = ampersand_pos - input;
        *bg = (char *)malloc(bg_length + 1);
        if (*bg == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        strncpy(*bg, input, bg_length);
        (*bg)[bg_length] = '\0';

        char *end = *bg + bg_length - 1;
        while (end >= *bg && (*end == ' ' || *end == '\t'))
        {
            *end = '\0';
            end--;
        }

        ampersand_pos++;

        while (*ampersand_pos == ' ' || *ampersand_pos == '\t')
        {
            ampersand_pos++;
        }

        *fg = strdup(ampersand_pos);
        if (*fg == NULL)
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        *bg = NULL;
        *fg = strdup(input);
        if (*fg == NULL)
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
    }
}

void execute_command(char *home, char *prev_dir, int home_size, char *cmd, char *his, FILE *history_file)
{
    while (*cmd == ' ' || *cmd == '\t')
    {
        cmd++;
    }
    if (*cmd != '\0')
    {
        if (strcmp(his, "pastevents") == 0 || strcmp(his, " pastevents") == 0 || strcmp(his, " pastevents ") == 0 || strcmp(his, "pastevents ") == 0)
        {

            display_history(history_file);
        }
        else if (strcmp(his, "pastevents purge") == 0)
        {
            pastevents_purge(&history_file);
        }
        else if (strncmp(his, "pastevents execute ", 19) == 0)
        {
            int n;
            if (sscanf(his + 19, "%d", &n) == 1 && n >= 1 && n <= 15)
            {
                past_exec(home, prev_dir, home_size, cmd, his, history_file, n);
            }
            else
            {
                printf("Invalid pastevents command: %s\n", his);
            }
        }
        else
        {
            char *sub_token = strtok(cmd, " ");
            int flag = 0;
            if (sub_token != NULL)
            {
                for (int i = 0; commands[i].command != NULL; i++)
                {
                    if (strcmp(sub_token, commands[i].command) == 0)
                    {
                        char *back;
                        char *front;

                        flag = 1;
                        commands[i].function(home, prev_dir, home_size, cmd + strlen(sub_token) + 1);
                        split_command(his, &back, &front);
                        add_to_history(front, history_file);
                        break;
                    }
                }
            }
            if (flag == 0)
            {
                printf("Invalid command: %s\n", cmd);
            }
        }
    }
    return;
}

void input_proc(char **list, int tokencount, char *home, char *prev_dir, int home_size, FILE *history_file)
{
    for (int i = 0; i < tokencount; i++)
    {
        char *bg;
        char *fg;

        split_command(list[i], &bg, &fg);

        if (bg != NULL)
        {
            if (strcmp(bg, "exit") == 0)
            {
                checkBackgroundProcesses();
                add_to_history(bg, history_file);
                break;
            }
            else
            {
                executeCommand(bg);
                add_to_history(bg, history_file);
                checkBackgroundProcesses();
            }
            execute_command(home, prev_dir, home_size, fg, list[i], history_file);
        }
        else
        {
            checkBackgroundProcesses();
            if (fg != NULL)
                execute_command(home, prev_dir, home_size, fg, list[i], history_file);
        }

        free(bg);
        free(fg);
    }
}
