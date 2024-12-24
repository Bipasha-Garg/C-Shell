#include "history.h"
#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOKEN_SIZE 100
#define HISTORY_SIZE 15
#define BUFFER_SIZE 512
#define HISTORY_FILE "history.txt"

void pastevents_purge(FILE **history_file_ptr)
{
    if (*history_file_ptr == NULL)
    {
        fprintf(stderr, "History file is not open\n");
        return;
    }
    fclose(*history_file_ptr);

    *history_file_ptr = fopen(HISTORY_FILE, "w");
    if (*history_file_ptr == NULL)
    {
        fprintf(stderr, "Failed to open history file\n");
        return;
    }

    fclose(*history_file_ptr);
}

int compareCommands(const char *command, const char *lastLine)
{
    char tokens1[100][TOKEN_SIZE];
    char *token1;
    int tokenCount1 = 0;

    char tokens2[100][TOKEN_SIZE];
    char *token2;
    int tokenCount2 = 0;

    token1 = strtok((char *)lastLine, " ");
    while (token1 != NULL && tokenCount1 < 100)
    {
        if (strlen(token1) >= TOKEN_SIZE)
        {
            // printf("Token too large\n");
            return 0;
        }
        strcpy(tokens1[tokenCount1], token1);
        tokenCount1++;
        token1 = strtok(NULL, " ");
    }

    // Tokenize the second string (command)
    token2 = strtok((char *)command, " ");
    while (token2 != NULL && tokenCount2 < 100)
    {
        if (strlen(token2) >= TOKEN_SIZE)
        {
            // printf("Token too large\n");
            return 0;
        }
        strcpy(tokens2[tokenCount2], token2);
        tokenCount2++;
        token2 = strtok(NULL, " ");
    }

    if (tokenCount1 != tokenCount2)
    {
        return 0;
    }

    for (int i = 0; i < tokenCount1; i++)
    {
        if (strcmp(tokens1[i], tokens2[i]) != 0)
        {
            return 0;
        }
    }

    // printf("not added\n");
    return 1; // Tokens are equal
}

void add_to_history(const char *command, FILE *readok)
{
    FILE *history_file = fopen(HISTORY_FILE, "r");
    if (history_file == NULL)
    {
        fprintf(stderr, "Failed to open history file for appending\n");
        return;
    }

    char *lastLine = (char *)malloc(BUFFER_SIZE);
    if (lastLine == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(history_file);
        return;
    }

    char line[512];
    int lastLineLength = 0;

    while (fgets(line, sizeof(line), history_file) != NULL)
    {
        strcpy(lastLine, line);
        lastLineLength = strlen(lastLine);
    }

    if (lastLineLength > 0)
    {
        if (lastLine[lastLineLength - 1] == '\n')
        {
            lastLine[lastLineLength - 1] = '\0';
        }

        // printf("Last line: %s\n", lastLine);
    }
    else
    {
        printf("File empty.\n");
    }

    fclose(history_file);

    int check = 0;
    char *printing = (char *)malloc(BUFFER_SIZE);
    if (printing == NULL)
    {
        fprintf(stderr, "Failed\n");
        free(lastLine);
        return;
    }
    strcpy(printing, command);
    check = compareCommands(command, lastLine);

    FILE *readme = fopen(HISTORY_FILE, "a");
    if (check != 1)
    {
        fprintf(readme, "%s\n", printing);
    }

    free(lastLine);
    free(printing);
    fclose(readme);
}

void display_history(FILE *readok)
{
    FILE *history_file = fopen(HISTORY_FILE, "r");
    if (history_file == NULL)
    {
        fprintf(stderr, "Failed to open history file for reading\n");
        return;
    }

    char line[512];
    char *lines[HISTORY_SIZE];
    int count = 0;

    for (int i = 0; i < HISTORY_SIZE; i++)
    {
        lines[i] = (char *)malloc(512 * sizeof(char));
        if (lines[i] == NULL)
        {
            fprintf(stderr, "ERROR ALLOCATING MEMORY\n");
            for (int j = 0; j < i; j++)
            {
                free(lines[j]);
            }
            fclose(history_file);
            return;
        }
    }

    while (fgets(line, sizeof(line), history_file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        strcpy(lines[count % HISTORY_SIZE], line);
        count++;
    }

    int start_index = 0;
    if (count > HISTORY_SIZE)
    {
        start_index = count - HISTORY_SIZE;
    }

    for (int i = 0; i < HISTORY_SIZE && i < count; i++)
    {
        printf("%d. %s\n", i + 1, lines[(start_index + i) % HISTORY_SIZE]);
    }

    for (int i = 0; i < HISTORY_SIZE; i++)
    {
        free(lines[i]);
    }

    fclose(history_file);
}

void past_exec(char *home, char *prev_dir, int home_size, char *cmd, char *his, FILE *history_file, int n)
{
    FILE *history_copy = fopen(HISTORY_FILE, "r");
    if (history_copy == NULL)
    {
        perror("Failed to open history file");
        return;
    }

    if (n >= 1 && n <= HISTORY_SIZE)
    {
        char past_events[BUFFER_SIZE][BUFFER_SIZE];
        int count = 0;

        while (fgets(past_events[count], sizeof(past_events[count]), history_copy) != NULL)
        {
            past_events[count][strcspn(past_events[count], "\n")] = '\0';
            count++;
        }

        if (n <= count)
        {
            // printf("Executing command: %s\n", past_events[count - n]);
            execute_command(home, prev_dir, home_size, past_events[count - n], past_events[count - n], history_file);
        }
        else if (n == count + 1)
        {
            // printf("Executing the last command: %s\n", past_events[count - 1]);
        }
        else
        {
            printf("Invalid index. Available history commands: 1 to %d.\n", count + 1);
        }
    }
    else
    {
        printf("Invalid index. Enter a number between 1 and 15.\n");
    }

    fclose(history_copy);
}
