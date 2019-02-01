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
#include "../common/random_generator.hpp"
#include "../common/h_function.hpp"
#include "./f_function.hpp"
#include "./vertex.hpp"
#include "./initialize/initialize_cube.hpp"
#include <bitset>

using namespace std;

template <class T>
class CubeExecution
{
  public:
    //Constructor
    CubeExecution()
    {
    }

    ~CubeExecution()
    {
        delete f_function_;
    }

    vector<ResultItem *> *find_nearest(
        Metrics<T> *&metric,
        DataItem<T> *&query,
        double R,
        unsigned int limit,
        InitializeCube *&params)
    {
        bool flag = false;
        ResultItem *temp;
        double distance = 0;
        string id;
        vector<ResultItem *> *result = new vector<ResultItem *>();

        id = f_function_->apply_function(query->vector_data);
        vector<string> *nearest_vertexes = find_nerest_vertexes(id, params->get_probes(), metric);

        for (int i = 0; i < params->get_probes(); i++)
        {
            for (unsigned int j = 0; j < vertexes_.size(); j++)
            {
                if (vertexes_.at(j)->id == nearest_vertexes->at(i))
                {
                    for (unsigned int k = 0; k < vertexes_.at(j)->bucket->size() && k < params->get_M(); k++)
                    {
                        distance = metric->get_distance(vertexes_.at(j)->bucket->at(k)->vector_data, query->vector_data);
                        if (distance <= R || R == 0)
                        {
                            if (result->size() < limit)
                            {
                                result->push_back(new ResultItem(vertexes_.at(j)->bucket->at(k)->id, distance));
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
                                    if (result->at(z)->id == vertexes_.at(j)->bucket->at(k)->id)
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
                                result->push_back(new ResultItem(vertexes_.at(j)->bucket->at(k)->id, distance));
                            }
                        }
                    }

                    break;
                }
            }
        }

        delete nearest_vertexes;
        return result;
    }

    void do_preprocessing(
        vector<DataItem<T> *> *&input_data,
        const int k,
        MetricType type)
    {
        //inits
        RandomGenerator *random_generator = new RandomGenerator();
        string id;

        f_function_ = get_f(input_data->at(0)->vector_data->size(), random_generator, k, type);

        int vertex_count = pow(2, k);
        for (int i = 0; i < vertex_count; i++)
        {
            string vrtx_id;
            // 100 is the max binary value
            vrtx_id = bitset<100>(i).to_string();
            vrtx_id.erase(vrtx_id.begin(), vrtx_id.end() - k);
            vertexes_.push_back(new Vertex<T>(vrtx_id));
        }

        // Pre-processing
        for (unsigned int j = 0; j < input_data->size(); j++)
        {
            id = f_function_->apply_function(input_data->at(j)->vector_data);

            for (unsigned int i = 0; i < vertexes_.size(); i++)
            {
                if (id == vertexes_.at(i)->id)
                {
                    vertexes_.at(i)->bucket->push_back(input_data->at(j));
                }
            }
        }

        // House keeping
        delete random_generator;
        // End
        return;
    }

  private:
    FFunction<T> *get_f(int dementions, RandomGenerator *&random_generator, int k, MetricType type)
    {
        // good constants
        int w = 4;
        FFunction<T> *result = new FFunction<T>(new vector<HFunction<T> *>());

        for (int i = 0; i < k; i++)
        {
            vector<double> *v = new vector<double>();

            for (int j = 0; j < dementions; j++)
            {
                v->push_back(random_generator->get_normal_distribution_random());
            }
            result->f->push_back(new HFunction<T>(v, random_generator->get_uniform_distribution_random(), w, type));
        }
        return result;
    }

    vector<string> *find_nerest_vertexes(string id, unsigned int count, Metrics<T> *&metric)
    {
        vector<string> all;
        vector<string> *result = new vector<string>();
        int current_similarity = 0, distance = 0;

        for (unsigned int i = 0; i < vertexes_.size(); i++)
        {
            all.push_back(vertexes_.at(i)->id);
        }

        while (result->size() < count)
        {
            for (unsigned int i = 0; i < all.size(); i++)
            {
                distance = metric->get_hamming_distance(id, all.at(i));
                if (distance == current_similarity)
                {
                    result->push_back(all.at(i));

                    if (result->size() == count)
                    {
                        return result;
                    }
                }
            }
            current_similarity++;
        }

        return result;
    }

    vector<Vertex<T> *> vertexes_;
    FFunction<T> *f_function_;
};

#endif