#include "file_managment.h"

char *get_random_message()
{
    // Initializations
    int line_count, random_index, current_count = 0;
    char *line = (char *)malloc(MAX_MESSAGE_SIZE * sizeof(char));

    FILE *fp;
    // Open the file
    fp = fopen(MESSAGES_FILE_NAME, "r");

    // Check if file exists
    if (fp == NULL)
    {
        perror("Couldnt open file: ");
        exit(1);
    }

    line_count = get_line_count(fp);
    random_index = rand() % (line_count + 1);

    while (fgets(line, MAX_MESSAGE_SIZE, fp) != NULL) /* read a line */
    {
        if (current_count == random_index)
        {
            fclose(fp);
            return line;
        }
        else
        {
            current_count++;
        }
    }
}

void write_to_log(char *data, int uid)
{
    FILE *fp;
    // Open the file
    char file_name[strlen(LOG_FILE_NAME)];
    strcpy(file_name, LOG_FILE_NAME);
    sprintf(&file_name[strlen(file_name) - 1], "%d", uid);

    fp = fopen(file_name, "a");

    // Check if file exists
    if (fp == NULL)
    {
        perror("Couldnt open file : ");
        exit(1);
    }
    fputs(data, fp);
    fclose(fp);
}

int get_line_count(FILE *fp)
{
    int count = -1; //because of the last blank line
    int ch;
    do
    {
        ch = fgetc(fp);
        if (ch == '\n')
            count++;
    } while (ch != EOF);

    rewind(fp);
    return count;
}
