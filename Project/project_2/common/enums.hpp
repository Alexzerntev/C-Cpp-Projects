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

enum InitializationType
{
    RANDOM_INITIALIZATION,
    KMEANS_PP_INITIALIZATION
};

enum AssignmentType
{
    LLOYDS_ASSIGNMENT,
    LSH_ASSIGNMENT,
    CUBE_ASSIGNMENT
};

enum UpdateType
{
    KMEANS_UPDATE,
    PAM_UPDATE
};
#endif