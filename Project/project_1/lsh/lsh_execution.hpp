#ifndef LSH_EXECUTION_HPP
#define LSH_EXECUTION_HPP

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../common/enums.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../common/metrics.hpp"
#include "../common/data_manipulation/result_item.hpp"
#include "./initialize/initialize_lsh.hpp"
#include "../common/random_generator.hpp"
#include "../common/h_function.hpp"
#include "./g_function.hpp"

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
        InitializeLsh *&params,
        MetricType type)
    {
        //inits
        RandomGenerator *random_generator = new RandomGenerator();

        // L iterations
        for (int i = 0; i < params->get_L(); i++)
        {
            GFunction<T> *g_function = get_g(input_data->at(0)->vector_data->size(), random_generator, params->get_k(), type);
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

    vector<ResultItem *> *find_nearest(
        Metrics<T> *&metric,
        DataItem<T> *&query,
        double R,
        unsigned int limit)
    {

        bool flag = false;
        ResultItem *temp;
        int bucket = 0;
        double distance = 0;
        vector<ResultItem *> *result = new vector<ResultItem *>();
        for (unsigned int i = 0; i < hash_tables_.size(); i++)
        {
            bucket = hash_tables_.at(i)->bucket(g_functions_.at(i)->apply_function(query->vector_data));

            for (auto it = hash_tables_.at(i)->begin(bucket); it != hash_tables_.at(i)->end(bucket); ++it)
            {
                distance = metric->get_distance(it->second->vector_data, query->vector_data);
                if (distance <= R || R == 0)
                {
                    if (result->size() < limit)
                    {
                        result->push_back(new ResultItem(it->second->id, distance));
                    }
                    else
                    {
                        sort(result->begin(), result->end(), compareResult);
                        if (result->at(result->size() - 1)->distance < distance)
                        {
                            continue;
                        }

                        for (unsigned int z = 0; z < result->size(); z++)
                        {
                            if (result->at(z)->id == it->second->id)
                            {
                                flag = true;
                                break;
                            }
                        }

                        if (flag == true)
                        {
                            flag = false;
                            continue;
                        }

                        temp = result->at(result->size() - 1);
                        result->pop_back();
                        delete temp;
                        result->push_back(new ResultItem(it->second->id, distance));
                    }
                }
            }
        }
        return result;
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