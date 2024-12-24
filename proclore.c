#include "headers.h"
#include "proclore.h"

void proc_details(char *extract, char *home, pid_t pid)
{
    // pid_t pid;
    char *column_2 = (char *)malloc(sizeof(char *) * 300);
    char *status = (char *)malloc(sizeof(char *) * 300);
    int column_22;
    pid_t column_4;
    pid_t column_7;
    pid_t column_8;
    char procFile[1000];
    char exeFile[1000];
    int i = 0;

    sprintf(procFile, "/proc/%d/stat", pid);
    sprintf(exeFile, "/proc/%d/exe", pid);
    FILE *statFile = fopen(procFile, "r");

    if (statFile == NULL)
    {
        perror("Error: pinfo");
        return;
    }

    int len = readlink(exeFile, status, 1000);

    if (len < 0)
    {
        perror("Error: pinfo");
        return;
    }

    status[len] = '\0';

    while (i < 23)
    {
        char temp[1000];
        fscanf(statFile, "%s", temp);

        switch (i)
        {
        case 2:
            strcpy(column_2, temp);
            break;
        case 4:
            column_4 = atoi(temp);
            break;
        case 7:
            column_7 = atoi(temp);
            break;
        case 8:
            column_8 = atoi(temp);
            break;
        case 22:
            column_22 = atoi(temp);
            break;
        default:
            break; // Handle other cases if needed
        }

        i++;
    }


    printf("pid : %d\n", pid);
    if (column_4 == column_7)
    {
        printf("Process Status : %c+\n", column_2[0]);
    }
    else
    {
        printf("Process Status : %c\n", column_2[0]);
    }
    printf("Process Group : %d\n", column_8);
    printf("Virtual memory : %d \n", column_22);
    if (strcmp(status, home) == 0)
    {
        printf("executable Path : ~\n");
    }
    else if (strlen(status) < strlen(home) || strlen(status) >= strlen(home) && strstr(status, home) == NULL)
    {
        printf("executable Path : %s\n", status);
    }
    else
    {
        int pointer = strlen(home);
        char path[1000];
        strcpy(path, &status[pointer]);
        printf("executable Path : ~%s\n", path);
    }


    // free(column_2);
    // free(status);
}

void proclore_func(char *home, int home_size, char *extract)
{
    pid_t pid;

    if (extract != NULL && strlen(extract) > 0)
    {
        pid = atoi(extract);
    }
    else
    {
        pid = getpid();
    }

    proc_details(extract, home, pid);
}
