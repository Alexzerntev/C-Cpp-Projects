#ifndef DATE_TIME_H
#define DATE_TIME_H
#define __USE_XOPEN 
#define _GNU_SOURCE

#include <time.h>
#include "../string/string_handler.h"

int is_between_date_time(char *date1,
                         char *time1,
                         char *date2,
                         char *time2,
                         char *date3,
                         char *time3);

int compare_date_time(char *date1, char *time1, char *date2, char *time2);

char *get_now_date(int *result_code);

char *get_now_time(int *result_code);

#endif