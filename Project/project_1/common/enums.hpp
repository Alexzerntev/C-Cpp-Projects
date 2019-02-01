#ifndef ENUMS_H
#define ENUMS_H
#include <iostream>
#include <string>

using namespace std;

enum MetricType
{
    EUCLIDEAN,
    COSINE
};

enum AttributeType
{
    INPUT = 0,
    OUTPUT,
    QUERY,
    LS_COUNT,
    HASH_TABLES,
    C_LS_COUNT,
    M_COUNT,
    PROBES
};

string get_atr_message(AttributeType type);

#endif