#include "./file_manager.h"

//==================================================================================================
//                                        dir functions
//==================================================================================================

int initialize_dirs(Settings *settings)
{
    int result_code = dir_exists(settings->input_dir);
    if (result_code == 0)
    {
        print_error(WRONG_INITIALIZATION_ERROR, "Wrong input directory (it does not exist)");
        return WRONG_INITIALIZATION_ERROR;
    }

    result_code = dir_exists(settings->mirror_dir);
    if (result_code == 1)
    {
        print_error(WRONG_INITIALIZATION_ERROR, "Wrong mirror directory (it already exists)");
        return WRONG_INITIALIZATION_ERROR;
    }
    else
    {
        result_code = create_dir(settings->mirror_dir);
        if (result_code != SUCCESS)
        {
            return result_code;
        }
    }

    result_code = dir_exists(settings->common_dir);
    if (result_code == 0)
    {
        result_code = create_dir(settings->common_dir);
        if (result_code != SUCCESS)
        {
            return result_code;
        }
    }
    return SUCCESS;
}

int dir_exists(char *dir_name)
{
    DIR *dir = opendir(dir_name);
    if (dir)
    {
        closedir(dir);
        return 1;
    }
    else if (ENOENT == errno)
    {
        /* Directory does not exist. */
        return 0;
    }
    else
    {
        printf("Error with directory : %s\n", dir_name);
        return DIRECTORY_ERROR;
    }
}

int create_dir(char *dir_name)
{
    int result = mkdir(dir_name, 0755);
    if (result == 0)
    {
        return SUCCESS;
    }
    else
    {
        printf("Directory name: %s\n", dir_name);
        print_error(COULD_NOT_CREATE_DIRECTORY, "");
        return COULD_NOT_CREATE_DIRECTORY;
    }
}

ClientListNode *initialize_clients(Settings *settings, int *result_code)
{
    char buffer[400];
    char name_buffer[400];
    sprintf(name_buffer, "%d.id", settings->id);
    size_t length = 0;
    FILE *fp = NULL;
    char *line = NULL;
    int pid, id;
    struct dirent *direntp;
    DIR *dir;
    ClientListNode *start_node = NULL;
    if (*result_code != SUCCESS)
    {
        return NULL;
    }

    if ((dir = opendir(settings->common_dir)) == NULL)
    {
        print_error(DIRECTORY_ERROR, "Initializing clients");
        return NULL;
    }
    while ((direntp = readdir(dir)) != NULL)
    {
        if (direntp->d_type == DT_REG && strstr(direntp->d_name, ".id") != NULL && strstr(name_buffer, direntp->d_name) == NULL)
        {
            strcpy(buffer, direntp->d_name);
            buffer[strlen(direntp->d_name) - 3] = '\0';
            id = atoi(buffer);
            if (id == 0 && strlen(buffer) != 1)
            {
                *result_code = INT_CONVERTION_ERROR;
                print_error(INT_CONVERTION_ERROR, "id convertion in client initialization");
                return NULL;
            }
            sprintf(buffer, "%s/%s", settings->common_dir, direntp->d_name);
            fp = fopen(buffer, "r");
            if (fp == NULL)
            {
                print_error(READ_FILE_ERROR, "Id file opening");
                fclose(fp);
                return NULL;
            }
            getline(&line, &length, fp);
            pid = atoi(line);
            if (pid == 0 && strlen(line) != 1)
            {
                print_error(INT_CONVERTION_ERROR, "Wrong pid saved in file");
                free(line);
                fclose(fp);
                return NULL;
            }

            *result_code = add_to_client_list(&start_node, id, pid, 1);
            if (*result_code != SUCCESS)
            {
                free(line);
                fclose(fp);
                return NULL;
            }

            free(line);
            fclose(fp);
        }
    }

    closedir(dir);
    return start_node;
}

void refresh_clients(ClientListNode **client_list, Settings *settings, int *result_code)
{
    char buffer[400];
    char name_buffer[400];
    sprintf(name_buffer, "%d.id", settings->id);
    size_t length = 0;
    FILE *fp = NULL;
    char *line = NULL;
    int pid, id;
    struct dirent *direntp;
    DIR *dir;
    ClientListNode *temp = NULL;
    if (*result_code != SUCCESS)
    {
        return;
    }

    if ((dir = opendir(settings->common_dir)) == NULL)
    {
        print_error(DIRECTORY_ERROR, "Initializing clients");
        return;
    }
    while ((direntp = readdir(dir)) != NULL)
    {
        if (direntp->d_type == DT_REG && strstr(direntp->d_name, ".id") != NULL && strstr(name_buffer, direntp->d_name) == NULL)
        {
            strcpy(buffer, direntp->d_name);
            buffer[strlen(direntp->d_name) - 3] = '\0';
            id = atoi(buffer);
            if (id == 0 && strlen(buffer) != 1)
            {
                *result_code = INT_CONVERTION_ERROR;
                print_error(INT_CONVERTION_ERROR, "id convertion in client initialization");
                return;
            }
            sprintf(buffer, "%s/%s", settings->common_dir, direntp->d_name);
            fp = fopen(buffer, "r");
            if (fp == NULL)
            {
                print_error(READ_FILE_ERROR, "Id file opening");
                return;
            }
            getline(&line, &length, fp);
            pid = atoi(line);
            if (pid == 0 && strlen(line) != 1)
            {
                print_error(INT_CONVERTION_ERROR, "Wrong pid saved in file");
                free(line);
                fclose(fp);
                return;
            }

            temp = find_client_by_id(*client_list, id);
            if (temp == NULL)
            {
                *result_code = add_to_client_list(client_list, id, pid, 1);
            }
            else
            {
                temp->dirty = 1;
            }

            if (*result_code != SUCCESS)
            {
                free(line);
                fclose(fp);
                return;
            }

            free(line);
            fclose(fp);
        }
    }
    closedir(dir);
    return;
}

ClientListNode *find_client_by_id(ClientListNode *start_node, int id)
{
    ClientListNode *temp = start_node;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

int map_input_dir(char *dir_name, char *relative_dir, int files_fd, int dirs_fd)
{
    char buffer[400];
    char buffer2[400];
    int result_code = SUCCESS;
    DIR *d;
    struct dirent *direntp;
    d = opendir(dir_name);
    if (d)
    {
        while ((direntp = readdir(d)) != NULL)
        {
            if (direntp->d_type == DT_REG && strcmp(direntp->d_name, "directory_list") != 0 && strcmp(direntp->d_name, "file_list") != 0)
            {
                sprintf(buffer2, "%s/%s", relative_dir, direntp->d_name);
                // +1 and -1 below is for understanding if its file or directory
                result_code = write(files_fd, buffer2 + 1, strlen(buffer2) - 1);
                if (result_code == -1)
                {
                    printf("Could not write to file with fd: %d\n", files_fd);
                    closedir(d);
                    return WRITE_TO_FILE_ERROR;
                }
                result_code = write(files_fd, "\n", 1);
                if (result_code == -1)
                {
                    printf("Could not write to file with fd: %d the line break symbol\n", files_fd);
                    closedir(d);
                    return WRITE_TO_FILE_ERROR;
                }
            }
            else if (direntp->d_type == DT_DIR && strstr(direntp->d_name, ".") == NULL)
            {
                sprintf(buffer2, "%s/%s", relative_dir, direntp->d_name);
                result_code = write(dirs_fd, buffer2, strlen(buffer2));
                if (result_code == -1)
                {
                    printf("Could not write to file with fd: %d\n", dirs_fd);
                    closedir(d);
                    return WRITE_TO_FILE_ERROR;
                }
                result_code = write(dirs_fd, "\n", 1);
                if (result_code == -1)
                {
                    printf("Could not write to file with fd: %d the line break symbol\n", dirs_fd);
                    closedir(d);
                    return WRITE_TO_FILE_ERROR;
                }
                sprintf(buffer, "%s/%s", dir_name, direntp->d_name);
                sprintf(buffer2, "%s/%s", relative_dir, direntp->d_name);
                result_code = map_input_dir(buffer, buffer2, files_fd, dirs_fd);
                if (result_code != SUCCESS)
                {
                    closedir(d);
                    return result_code;
                }
            }
        }
        closedir(d);
    }
    else
    {
        printf("Could not open directory : %s\n", dir_name);
        return DIRECTORY_ERROR;
    }

    return SUCCESS;
}

//==================================================================================================
//                                        file functions
//==================================================================================================
int create_id_file(char *file_name)
{
    char tmp[12] = {0x0};

    int result_code = SUCCESS;
    int pid = getpid();

    if (access(file_name, F_OK) != -1)
    {
        // file exists
        printf("Id file already exist in common\n");
        return -1;
    }

    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (fd == -1)
    {
        printf("Could not open file : %s", file_name);
        return FILE_OPENING_ERROR;
    }

    sprintf(tmp, "%d", pid);
    result_code = write(fd, tmp, strlen(tmp));
    if (result_code == -1)
    {
        printf("Could not write to a file : %s\n", file_name);
        return FILE_OPENING_ERROR;
    }

    result_code = close(fd);
    if (result_code == -1)
    {
        printf("Could not close a file : %s\n", file_name);
        return FILE_CLOSING_ERROR;
    }
    return SUCCESS;
}

//==================================================================================================
//                                        other
//=================================================================================================

void clean_files(Settings *settings)
{
    char buffer[200];
    sprintf(buffer, "rm -rf %s", settings->mirror_dir);
    system(buffer);
    sprintf(buffer, "rm %s/%d.id", settings->common_dir, settings->id);
    system(buffer);
}
