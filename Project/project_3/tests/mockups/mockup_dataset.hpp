#ifndef MOCKUP_DATASET_HPP
#define MOCKUP_DATASET_HPP

#include <string>
#include <iostream>
#include <vector>
#include "../../common/data_manipulation/data_item.hpp"
#include "../../common/random_generator.hpp"
#include "../../common/metrics.hpp"
#include "../../common/enums.hpp"

using namespace std;

template <class T>
class MockUpDataset
{
  private:
  public:
    vector<DataItem<T> *> *dataset;
    Metrics<T> *eu_metrics;
    Metrics<T> *cos_metrics;

    MockUpDataset()
    {
        dataset = new vector<DataItem<T> *>();
        RandomGenerator random_generator;

        for (int i = 0; i < 10; i++)
        {
            DataItem<T> *item = new DataItem<T>();
            item->id = i + 1;
            item->vector_data = new vector<T>();
            item->is_known = new vector<bool>();

            for (unsigned int j = 0; j < 2; j++)
            {
                item->vector_data->push_back(i);
                item->is_known->push_back(true);
            }

            dataset->push_back(item);
        }
        eu_metrics = new Metrics<T>(EUCLIDEAN);
        cos_metrics = new Metrics<T>(COSINE);
    }

    ~MockUpDataset()
    {
        DataItem<double> *temp;
        while (dataset->size() > 0)
        {
            temp = dataset->at(0);
            dataset->erase(dataset->begin());
            delete temp;
        }
        delete dataset;
        delete eu_metrics;
        delete cos_metrics;
    }
};

#endif
