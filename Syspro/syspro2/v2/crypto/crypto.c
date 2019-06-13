#include "./crypto.h"

void generate_key(char *file_name, Settings *settings)
{
    char buffer[1000];
    sprintf(buffer, "echo Key-Type: default > batch_%d", settings->id);
    system(buffer);

    sprintf(buffer, "echo Subkey-Type: default >> batch_%d", settings->id);
    system(buffer);

    sprintf(buffer, "echo Name-Real: client_%d >> batch_%d", settings->id, settings->id);
    system(buffer);

    sprintf(buffer, "echo Name-Email: client_%d@gmail.com >> batch_%d", settings->id, settings->id);
    system(buffer);

    sprintf(buffer, "gpg --batch --gen-key batch_%d", settings->id);
    system(buffer);
}

void remove_keys(int id)
{
    char buffer[1000];
    sprintf(buffer, "gpg --yes --delete-secret-key client_%d", id);
    system(buffer);
    sprintf(buffer, "gpg --yes --delete-key client_%d", id);
    system(buffer);
}

void encrypt(char *file_name, int id)
{
    char buffer[1000];

    sprintf(buffer, "gpg -r client_%d --encrypt %s", id, file_name);
    printf("%s\n", buffer);
    system(buffer);
}

void decrypt(char *file_name, int id)
{
    char buffer[1000];
    char bf[500];
    char *ptr;
    sprintf(bf, "%s", file_name);

    ptr = strstr(bf, ".gpg");
    *ptr = '\0';

    sprintf(buffer, "gpg --output %s --decrypt %s", bf, file_name);
    printf("%s\n", buffer);
    system(buffer);

    sprintf(buffer, "rm %s", file_name);
    system(buffer);
}