#include "headers.h"
#include "prompt.h"
void prompt(char *home, int home_size)
{
    char curr_dir[MAX_ARR_SIZE];
    // char username[MAX_ARR_SIZE];
    char hostname[MAX_ARR_SIZE];

    char *username = getenv("USER");
    gethostname(hostname, sizeof(hostname));
    if (getcwd(curr_dir, sizeof(curr_dir)) == NULL)
    {
        perror("error");
        return;
    }
    // printf("%s\n", curr_dir);
    // printf("%s\n", home);
    // printf("%d\n", home_size);
    if (strcmp(home, curr_dir) == 0)
    {
        printf("<%s@%s:~>", username, hostname);
    }
    else if (strlen(curr_dir) < home_size)
    {
        //doubt: WHAT SHOULD BE PRINTED?
        printf("<%s@%s:~>", username, hostname);
    }
    else
    {
        printf("<%s@%s:~%s>", username, hostname, curr_dir + home_size);
    }       

}
