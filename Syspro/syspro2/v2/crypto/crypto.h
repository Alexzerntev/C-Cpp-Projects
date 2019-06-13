#ifndef CRYPTO_H
#define CRYPTO_H

#include "string.h"
#include <stdio.h>
#include "../model/settings/settings.h"

void generate_key(char *file_name, Settings *settings);

void remove_keys(int id);

void encrypt(char *file_name, int id);

void decrypt(char *file_name, int id);

#endif