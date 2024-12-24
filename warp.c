#include "warp.h"
#include "headers.h"

char buffer[10000];

void warp_ok(char home_directory[500], char prev_dir[500], int home_size, char *extract)
{
    // get current working directory
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("ERROR 1 IN GETTING CWD IN WARP.C");
        return;
    }

    // set buffer to null
    memset(buffer, '\0', sizeof(char) * 10000);

    // tokenize
    extract = strtok(NULL, " ");

    while (extract != NULL)
    {
        // if - then move to prev directory
        if (strcmp(extract, "-") == 0)
        {

            char temp[1024];
            strcpy(temp, prev_dir);
            if (chdir(prev_dir) == -1)
            {
                perror("ERROR 1 IN CHANGING DIRECTORY IN WARP.C");
            }
            else
            {
                strcpy(prev_dir, cwd);
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    printf("%s\n", cwd);
                }
                else
                {
                    perror("ERROR 2 IN GETTING CWD IN WARP.C");
                }
            }
        }
        else if (strcmp(extract, ".") == 0)
        {
            // Do nothing
        }
        else if (strcmp(extract, "..") == 0)
        {
            if (chdir("..") == -1)
            {
                perror("ERROR 2 IN CHANGING DIRECTORY IN WARP.C");
            }
            else
            {
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    printf("%s\n", cwd);
                }
                else
                {
                    perror("ERROR 3 IN GETTING CWD IN WARP.C");
                }
            }
        }
        else if (strcmp(extract, "~") == 0)
        {
            // Change to the home directory
            if (chdir(home_directory) == -1)
            {
                perror("ERROR 3 IN CHANGING DIRECTORY IN WARP.C");
            }
            else
            {
                strcpy(prev_dir, cwd);
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    printf("%s\n", cwd);
                }
                else
                {
                    perror("ERROR 4 IN GETTING CWD IN WARP.C");
                }
            }
        }
        else if (strncmp(extract, "~/", 2) == 0)
        {
            // Get user's home directory
            struct passwd *pw = getpwnam(getlogin());
            if (pw != NULL)
            {
                char *home_path = pw->pw_dir;
                getcwd(buffer, sizeof(buffer));
                int buff_len = strlen(buffer);
                buffer[buff_len] = '\0';
                strcat(buffer, home_path);
                strcat(buffer, &extract[1]); 

                // F_OK: Test for existence
                if (access(buffer, F_OK) != -1)
                {
                    if (chdir(buffer) == -1)
                    {
                        perror("ERROR 4 IN CHANGING DIRECTORY IN WARP.C");
                    }
                    else
                    {
                        if (getcwd(cwd, sizeof(cwd)) != NULL)
                        {
                            printf("%s\n", cwd);
                        }
                        else
                        {
                            perror("ERROR 5 IN GETTING CWD IN WARP.C");
                        }
                    }
                }
                else
                {
                    printf("DIRECTORY DOES NOT EXIST: %s\n", buffer);
                }
            }
            else
            {
                perror("ERROR IN GETPWNAM IN WARP.C\n");
                printf("FAILED TO GET USER'S DIRECTORY\n");
            }
        }
        else
        {
            getcwd(buffer, sizeof(buffer));
            int buff_len = strlen(buffer);
            buffer[buff_len] = '/';
            buffer[buff_len + 1] = '\0';
            strcat(buffer, extract);

            if (chdir(buffer) == -1)
            {
                perror("ERROR 5 IN CHANGING DIRECTORY IN WARP.C");
            }
            else
            {
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    printf("%s\n", cwd);
                }
                else
                {
                    perror("ERROR 6 IN GETTING CWD IN WARP.C");
                }
            }
        }
        extract = strtok(NULL, " ");
    }
}
