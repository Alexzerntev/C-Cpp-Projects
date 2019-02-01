#ifndef LSH_EXECUTION_HPP
#define LSH_EXECUTION_HPP

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "./g_function.hpp"
#include "../knn_common/h_function.hpp"
#include "../../clustering/cluster_helpers.hpp"
#include "../../common/enums.hpp"
#include "../../common/metrics.hpp"
#include "../../common/random_generator.hpp"
#include "../../common/data_manipulation/data_item.hpp"
#include "../../common/data_manipulation/data_manager.hpp"

using namespace std;

template <class T>
class LshExecution
{
  public:
    //Constructor
    LshExecution()
    {
    }

    ~LshExecution()
    {
        GFunction<T> *temp_g;
        unordered_multimap<string, DataItem<T> *> *temp_hash;

        while (hash_tables_.size() > 0)
        {
            temp_hash = hash_tables_.at(0);
            hash_tables_.erase(hash_tables_.begin());
            delete temp_hash;
        }

        while (g_functions_.size() > 0)
        {
            temp_g = g_functions_.at(0);
            g_functions_.erase(g_functions_.begin());
            delete temp_g;
        }
    }

    // Find Nearest By Euclidean
    void do_preprocessing(
        vector<DataItem<T> *> *&input_data,
        DataManager<T> *&params,
        MetricType type)
    {
        //inits
        RandomGenerator *random_generator = new RandomGenerator();

        // L iterations
        for (int i = 0; i < params->get_number_of_hash_tables(); i++)
        {
            GFunction<T> *g_function = get_g(input_data->at(0)->vector_data->size(), random_generator, params->get_number_of_hash_functions(), type);
            unordered_multimap<string, DataItem<T> *> *hash_table = new unordered_multimap<string, DataItem<T> *>();
            hash_table->max_load_factor(input_data->size() / sqrt(input_data->size()));

            // Pre-processing
            for (unsigned long j = 0; j < input_data->size(); j++)
            {
                hash_table->insert(make_pair(g_function->apply_function(input_data->at(j)->vector_data), input_data->at(j)));
            }

            hash_tables_.push_back(hash_table);
            g_functions_.push_back(g_function);
        }

        // House keeping apply_function
        delete random_generator;
        // End
        return;
    }

    int find_nearest(
        Metrics<T> *&metric,
        DataItem<T> *&query,
        double R,
        int cluster_index,
        vector<Cluster<T> *> *&clusters,
        bool is_first_time)
    {
        int points_found = 0;
        int bucket = 0;
        double distance = 0;
        for (unsigned int i = 0; i < hash_tables_.size(); i++)
        {
            bucket = hash_tables_.at(i)->bucket(g_functions_.at(i)->apply_function(query->vector_data));

            for (auto it = hash_tables_.at(i)->begin(bucket); it != hash_tables_.at(i)->end(bucket); ++it)
            {
                // Return if nothing to do
                if (!is_first_time && it->second->is_dirty == true)
                {
                    continue;
                }

                distance = metric->get_distance(it->second->vector_data, query->vector_data);

                // Return if out of distance
                if (distance >= R)
                {
                    continue;
                }

                // Check for collisions
                if (is_first_time)
                {
                    if (it->second->is_dirty)
                    {
                        if (it->second->with_distance > distance)
                        {
                            it->second->with_distance = distance;
                            delete_by_id(clusters->at(it->second->current_cluster), it->second->id);
                            it->second->current_cluster = cluster_index;
                            clusters->at(cluster_index)->data->push_back(it->second);
                            points_found++;
                        }
                        continue;
                    }
                }

                // If passed all the checks add it to cluster
                it->second->is_dirty = true;
                it->second->with_distance = distance;
                it->second->current_cluster = cluster_index;
                clusters->at(cluster_index)->data->push_back(it->second);
                points_found++;
            }
        }
        return points_found;
    }

  private:
    GFunction<T> *get_g(int dementions, RandomGenerator *&random_generator, int k, MetricType type)
    {
        // good constants
        int w = 4;
        double rnd;
        GFunction<T> *result = new GFunction<T>(new vector<HFunction<T> *>());

        for (int i = 0; i < k; i++)
        {
            vector<double> *v = new vector<double>();

            for (int j = 0; j < dementions; j++)
            {
                rnd = random_generator->get_normal_distribution_random();

                if (type == EUCLIDEAN)
                {
                    rnd = rnd + 3;
                    if (rnd < 0)
                    {
                        rnd = 0;
                    }
                }
                v->push_back(rnd);
            }
            result->g->push_back(new HFunction<T>(v, random_generator->get_uniform_distribution_random(), w, type));
        }
        return result;
    }

    vector<unordered_multimap<string, DataItem<T> *> *> hash_tables_;
    vector<GFunction<T> *> g_functions_;
};

#endif