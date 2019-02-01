#ifndef SILHOUETTE_HPP
#define SILHOUETTE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "../clustering/cluster.hpp"
#include "../common/metrics.hpp"
#include "../common/data_manipulation/data_item.hpp"

using namespace std;

template <class T>
class Silhouette
{
  private:
  public:
    // Constructors
    Silhouette()
    {
    }

    void evaluate(vector<Cluster<T> *> *&clusters, vector<DataItem<T> *> *&dataset, Metrics<T> *&metrics)
    {
        int nearest_cluster = 0;
        double cluster_average = 0;
        double nearest_cluster_average = 0;

        for (unsigned int i = 0; i < dataset->size(); i++)
        {
            cluster_average = get_cluster_average(clusters->at(dataset->at(i)->current_cluster), dataset->at(i), metrics);
            nearest_cluster = get_nearest_cluster(clusters, metrics, dataset->at(i)->current_cluster);
            nearest_cluster_average = get_cluster_average(clusters->at(nearest_cluster), dataset->at(i), metrics);
            dataset->at(i)->silhouette = (nearest_cluster_average - cluster_average) / max(cluster_average, nearest_cluster_average);
        }
        for (unsigned int i = 0; i < clusters->size(); i++)
        {
            double sum = 0;
            for (unsigned int j = 0; j < clusters->at(i)->data->size(); j++)
            {
                sum += clusters->at(i)->data->at(j)->silhouette;
            }
            clusters->at(i)->average_silhouette = sum / (clusters->at(i)->data->size());
        }

        return;
    }

    double get_cluster_average(Cluster<T> *&cluster, DataItem<T> *&data_item, Metrics<T> *&metrics)
    {
        double distance = 0;
        for (unsigned int i = 0; i < cluster->data->size(); i++)
        {
            distance += metrics->get_distance(cluster->data->at(i)->vector_data, data_item->vector_data);
        }
        return distance / cluster->data->size();
    }

    int get_nearest_cluster(vector<Cluster<T> *> *&clusters, Metrics<T> *&metrics, unsigned int index)
    {
        double min_distance = 99999999;
        double distance = 0;
        int nearest_cluster = 0;
        for (unsigned int i = 0; i < clusters->size(); i++)
        {
            if (i == index)
            {
                continue;
            }

            distance = metrics->get_distance(clusters->at(index)->centroid->vector_data, clusters->at(i)->centroid->vector_data);
            if (distance < min_distance)
            {
                min_distance = distance;
                nearest_cluster = i;
            }
        }
        return nearest_cluster;
    }
};

#endif
