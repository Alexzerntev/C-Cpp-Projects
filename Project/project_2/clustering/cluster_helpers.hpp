#ifndef CLUSTER_HELPERS_HPP
#define CLUSTER_HELPERS_HPP

#include <string>
#include <iostream>
#include <vector>
#include "./cluster.hpp"
#include "../common/random_generator.hpp"
#include "../common/metrics.hpp"
#include "../common/enums.hpp"
#include "../common/argument_initialize.hpp"
#include "../common/data_manipulation/data_manager.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../knn/lsh/lsh_execution.hpp"
#include "../knn/cube/cube_execution.hpp"

using namespace std;

template <class T>
void delete_by_id(Cluster<T> *&cluster, unsigned long int id)
{
    for (unsigned int i = 0; i < cluster->data->size(); i++)
    {
        if (cluster->data->at(i)->id == id)
        {
            cluster->data->erase(cluster->data->begin() + i);
            return;
        }
    }
}

template <class T>
bool is_center(vector<Cluster<T> *> *&clusters, unsigned long int id)
{
    for (unsigned int i = 0; i < clusters->size(); i++)
    {
        if (clusters->at(i)->centroid->id == id)
        {
            return true;
        }
    }
    return false;
}
#endif