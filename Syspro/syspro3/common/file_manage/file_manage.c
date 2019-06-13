#include "./file_manage.h"

// should be disposed by the caller
char *get_hash_of_file(char *filename)
{
    int result_code = SUCCESS;
    unsigned char c[MD5_DIGEST_LENGTH];
    char c2[2 * MD5_DIGEST_LENGTH];
    FILE *inFile = fopen(filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (inFile == NULL)
    {
        printf("%s can't be opened.\n", filename);
        return NULL;
    }

    MD5_Init(&mdContext);
    while ((bytes = fread(data, 1, 1024, inFile)) != 0)
    {
        MD5_Update(&mdContext, data, bytes);
    }

    MD5_Final(c, &mdContext);

    // for (i = 0; i < MD5_DIGEST_LENGTH; i++)
    // {
    //     printf("%02x", c[i]);
    // }

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) // 16 hex chars
    {
        sprintf(c2 + 2 * i, "%02x", c[i]);
    }

    char *to_return = create_string(2 * MD5_DIGEST_LENGTH, &result_code);
    memcpy(to_return, c2, 2 * MD5_DIGEST_LENGTH);
    fclose(inFile);
    return to_return;
}

int check_if_same_version(char *file_path, char *version)
{
    char *get_hash = get_hash_of_file(file_path);
    if (get_hash == NULL)
    {
        return 0;
    }
    if (strstr(version, get_hash) != NULL)
    {
        destroy_string(get_hash);
        return 1;
    }
    destroy_string(get_hash);
    return 0;
}

int file_exists(char *file_path)
{
    if (access(file_path, F_OK) != -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
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
        /* opendir() failed for some other reason. */
        return 0;
    }
}

int create_custom_dir(char *path)
{
    int result_code = mkdir(path, 0755);
    if (result_code < 0)
    {
        printf("Dir creation for path : %s", path);
        print_error(COULD_NOT_CREATE_DIRECTORY, "Mirror for other id");
        return COULD_NOT_CREATE_DIRECTORY;
    }
    return SUCCESS;
}

int map_input_dir(char *dir_name, char *relative_dir, int files_fd)
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
                sprintf(buffer, "%s/%s", dir_name, direntp->d_name);
                result_code = map_input_dir(buffer, buffer2, files_fd);
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

unsigned int get_file_size(char *file_name, int *result_code)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("Could not open file to get file size : %s\n", file_name);
        *result_code = COULD_NOT_OPEN_FILE;
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    unsigned int size = ftell(fp);
    fclose(fp);
    return size;
}

int get_line_count(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    char c;
    int count = 0;

    // Check if file exists
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }

    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;

    // Close the file
    fclose(fp);
    return count;
}

void create_recursive_dir(char *initial_dir)
{
    char *ts1 = strdup(initial_dir);

    char *dir = dirname(ts1);

    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/')
        {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);

    free(ts1);
    return;
}
