#include "headers.h"
#include "seek.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

void print_color(char *text, char *color)
{
    printf("\033[%sm%s\033[0m", color, text);
}

void particular(char *target_dir, char *name, int flags)
{
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(target_dir)) == NULL)
    {
        perror("opendir");
        return;
    }

    int found = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        char full_path[1024];
        struct stat stat_buf;

        snprintf(full_path, sizeof(full_path), "%s/%s", target_dir, entry->d_name);

        if (stat(full_path, &stat_buf) == -1)
        {
            perror("stat");
            continue;
        }

        int is_dir = S_ISDIR(stat_buf.st_mode);

        if (((flags & 1) && !is_dir) || ((flags & 2) && is_dir))
            continue;

        if (name[0] != '\0' && strcmp(entry->d_name, name) == 0)
        {
            found = 1;
            printf("%s\n", full_path);
        }

        if (is_dir && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            particular(full_path, name, flags);
        }
    }

    closedir(dir);

    if (!found)
    {
        printf("No match found!\n");
    }
}

void call_seek(char *input)
{
    char *target_dir = ".";
    char *name = NULL;
    int flags = 0;

    char *token;
    char *arr[1024]; // max 1024 arguments
    int tokencount = 0;

    token = strtok(input, " ");
    while (token != NULL)
    {
        arr[tokencount] = token;
        tokencount++;
        token = strtok(NULL, " ");
    }

    if (tokencount < 1)
    {
        printf("Usage: %s [-d] [-f] [-e] <name> [<target_dir>]\n", arr[0]);
        return;
    }

    for (int i = 1; i < tokencount; i++)
    {
        if (strcmp(arr[i], "-d") == 0)
        {
            flags |= 1;
        }
        else if (strcmp(arr[i], "-f") == 0)
        {
            flags |= 2;
        }
        else if (strcmp(arr[i], "-e") == 0)
        {
            flags |= 4;
        }
        else
        {
            if (name == NULL)
            {
                name = strdup(arr[i]);
            }
            else
            {
                target_dir = strdup(arr[i]);
            }
        }
    }

    if (name == NULL)
    {
        printf("No name provided!\n");
    }
    else
    {
        particular(target_dir, name, flags);
    }

    if (name != NULL)
    {
        free(name);
    }
    if (target_dir != NULL && target_dir != ".")
    {
        free(target_dir);
    }
}
