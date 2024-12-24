#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define RESET "\x1B[0m"
#define GREEN "\x1B[32m"
#define BLUE "\x1B[34m"

int compare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void color_print(char *text, char *color)
{
    printf("%s%s%s", color, text, RESET);
}

void peek_func(char *extract, bool show_hidden, bool show_details)
{
    char *directory = NULL;

    // Handle special symbols and paths
    if (extract == NULL)
    {
        directory = getcwd(NULL, 0); // Current working directory
    }
    else if (strcmp(extract, ".") == 0)
    {
        directory = getcwd(NULL, 0); // Current working directory
    }
    else if (strcmp(extract, "..") == 0)
    {
        char *current_dir = getcwd(NULL, 0);
        char *last_slash = strrchr(current_dir, '/');
        if (last_slash != NULL)
        {
            *last_slash = '\0';
        }
        directory = current_dir;
    }
    else if (strcmp(extract, "~") == 0)
    {
        // Handle "~" as the user's home directory
        directory = getenv("HOME");
    }
    else if (strcmp(extract, "-") == 0)
    {
        // Handle "-" as the previous working directory
        directory = getenv("OLDPWD");
    }
    else if (extract[0] == '/')
    {
        // Absolute path
        directory = strdup(extract);
    }
    else
    {
        // Relative path
        char *current_dir = getcwd(NULL, 0);
        directory = (char *)malloc(strlen(current_dir) + strlen(extract) + 2);
        if (directory == NULL)
        {
            perror("malloc");
            return;
        }
        sprintf(directory, "%s/%s", current_dir, extract);
    }

    struct dirent *dir;

    DIR *direct = opendir(directory);
    if (direct == NULL)
    {
        perror("opendir");
        // free(directory);
        return;
    }

    char *array[1000];
    int i = 0;

    while ((dir = readdir(direct)) != NULL)
    {

        if (!show_hidden && dir->d_name[0] == '.')
        {
            continue;
        }
        array[i] = malloc(50000 * sizeof(char));
        if (array[i] == NULL)
        {
            perror("malloc");
            closedir(direct);
            // free(directory);
            return;
        }
        strcpy(array[i], dir->d_name);
        i++;
    }

    closedir(direct);

    qsort(array, i, sizeof(char *), compare);

    for (int j = 0; j < i; ++j)
    {
        char *file_name = array[j];
        bool is_directory = false;

        // Check if the entry is a directory
        struct stat file_stat;
        char full_path[2048];
        snprintf(full_path, 2048, "%s/%s", directory, file_name);
        if (stat(full_path, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
        {
            is_directory = true;
        }

        if (show_details)
        {
            if (is_directory)
            {
                color_print("d", BLUE);
            }
            else
            {
                color_print("-", GREEN);
            }
            printf("%c%c%c%c%c%c%c%c%c%c ",
                   (file_stat.st_mode & S_IRUSR) ? 'r' : '-',
                   (file_stat.st_mode & S_IWUSR) ? 'w' : '-',
                   (file_stat.st_mode & S_IXUSR) ? 'x' : '-',
                   (file_stat.st_mode & S_IRGRP) ? 'r' : '-',
                   (file_stat.st_mode & S_IWGRP) ? 'w' : '-',
                   (file_stat.st_mode & S_IXGRP) ? 'x' : '-',
                   (file_stat.st_mode & S_IROTH) ? 'r' : '-',
                   (file_stat.st_mode & S_IWOTH) ? 'w' : '-',
                   (file_stat.st_mode & S_IXOTH) ? 'x' : '-',
                   (file_stat.st_mode & S_ISVTX) ? 't' : '-');
            printf("%2ld ", file_stat.st_nlink);

            struct passwd *t = getpwuid(file_stat.st_uid);
            struct group *g = getgrgid(file_stat.st_gid);
            if (t != NULL && g != NULL)
            {
                printf("%s\t%s\t", t->pw_name, g->gr_name);
            }
            else
            {
                printf("unknown\tunknown\t");
            }

            printf("%lld\t", (long long)file_stat.st_size);

            struct tm *time = localtime(&(file_stat.st_mtime));
            char buffer1[40];
            if (strftime(buffer1, 40, "%b %d %H:%M", time) > 0)
            {
                printf("%s\t", buffer1);
            }
            else
            {
                printf("unknown\t");
            }
        }

        if (is_directory)
        {
            color_print(file_name, BLUE);
            printf("/");
        }
        else if (access(directory, R_OK) != 0)
        {
            fprintf(stderr, "No read access to directory: %s\n", directory);
            // Handle this case gracefully
            closedir(direct);
            // free(directory);
            return;
        }
        else if (access(full_path, X_OK) == 0)
        {
            color_print(file_name, GREEN);
        }
        else
        {
            printf("%s", file_name); // White for regular files
        }

        printf("\n");
    }

    // Free the memory allocated for the array and directory
    for (int j = 0; j < i; ++j)
    {
        free(array[j]);
    }
    // free(directory);
}

void peek(char *home, char *prev_dir, int home_size, char *extract)
{
    // Extract arguments
    int tokencount = 0;
    char *arr;
    bool show_hidden = false;
    bool show_details = false;
    char *path = NULL;
    char *cmd = strtok(extract, " "); // Use " " as the delimiter and initialize cmd
    // while (cmd != NULL)
    // {
    //     arr[tokencount] = cmd;
    //     tokencount++;
    //     cmd = strtok(NULL, " ");
    // }
    while (cmd != NULL)
    {
        if (strcmp(cmd, "-a") == 0)
        {
            show_hidden = true;
        }
        else if (strcmp(cmd, "-l") == 0)
        {
            show_details = true;
        }
        else
        {
            path = cmd;
        }
        cmd = strtok(NULL, " "); // Get the next token
    }

    // Handle the combined flags
    if (strstr(extract, "-al") || strstr(extract, "-la"))
    {
        show_hidden = true;
        show_details = true;
    }
    else if (strstr(extract, "-a"))
    {
        show_hidden = true;
    }
    else if (strstr(extract, "-l"))
    {
        show_details = true;
    }

    // Call the peek function with the appropriate flags
    peek_func(path, show_hidden, show_details);

    if (path != NULL)
    {
        free(path);
    }
}
