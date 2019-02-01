#ifndef CLUSTER_INITIALIZE_HPP
#define CLUSTER_INITIALIZE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include "./cluster.hpp"
#include "../common/random_generator.hpp"
#include "../common/metrics.hpp"

using namespace std;

template <class T>
class ClusterInitialize
{
  private:
  public:
    vector<Cluster<T> *> *initialize_cluster(
        int k,
        vector<DataItem<double> *> *&dataset,
        InitializationType type,
        Metrics<T> *&metrics)
    {
        vector<Cluster<T> *> *clusters = new vector<Cluster<T> *>();
        RandomGenerator *random_generator = new RandomGenerator();
        unsigned int center;
        vector<double> min_distances;
        min_distances.push_back(0);
        double current_min_distance = 0;
        double random_probability = 0;
        // If selected random initialization
        if (type == RANDOM_INITIALIZATION)
        {
            for (int i = 0; i < k; i++)
            {
                do
                {
                    center = random_generator->get_uniform_distribution_random_with_size((double)dataset->size());
                } while (dataset->at(center)->is_dirty);

                dataset->at(center)->is_dirty = true;
                dataset->at(center)->current_cluster = i;
                clusters->push_back(new Cluster<T>(i, dataset->at(center)));
            }
        }
        else
        {
            do
            {
                center = random_generator->get_uniform_distribution_random_with_size((double)dataset->size());
            } while (dataset->at(center)->is_dirty);

            dataset->at(center)->is_dirty = true;
            dataset->at(center)->current_cluster = 0;
            clusters->push_back(new Cluster<T>(0, dataset->at(center)));

            for (int i = 1; i < k; i++)
            {
                for (unsigned int j = 1; j < dataset->size(); j++)
                {
                    if (dataset->at(j)->is_dirty)
                    {
                        current_min_distance = 0;
                    }
                    else
                    {
                        current_min_distance = pow(get_min_distance(metrics, dataset, center), 2);
                    }
                    min_distances.push_back(min_distances.at(j - 1) + current_min_distance);
                }

                random_probability = random_generator->get_uniform_distribution_random_with_size(min_distances.at(min_distances.size() - 1));
                for (unsigned int j = 0; j < min_distances.size(); j++)
                {
                    if (random_probability <= min_distances.at(j) && !dataset->at(j)->is_dirty)
                    {
                        center = j;
                        break;
                    }
                }

                dataset->at(center)->is_dirty = true;
                dataset->at(center)->current_cluster = i;
                clusters->push_back(new Cluster<T>(i, dataset->at(center)));
                min_distances.clear();
                min_distances.push_back(0);
            }
        }

        delete random_generator;
        return clusters;
    }

    double get_min_distance(Metrics<T> *&metrics, vector<DataItem<double> *> *&dataset, unsigned int center)
    {
        double min_distance = 99999999;
        double current_distance;
        for (unsigned int i = 0; i < dataset->size(); i++)
        {
            if (center == i)
            {
                continue;
            }

            current_distance = metrics->get_distance(dataset->at(center)->vector_data, dataset->at(i)->vector_data);

            if (current_distance < min_distance)
            {
                min_distance = current_distance;
            }
        }
        return min_distance;
    }

    ClusterInitialize()
    {
    }

    ~ClusterInitialize()
    {
    }
};

#endif