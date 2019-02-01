#ifndef BF_EXECUTION_HPP
#define BF_EXECUTION_HPP

#include <string>
#include <iostream>
#include <vector>
#include "../common/enums.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../common/metrics.hpp"
#include "../common/data_manipulation/result_item.hpp"

using namespace std;

template <class T>
class BfExecution
{
private:
  bool has_errors_;

public:
  // Constructor
  BfExecution() {}

  // Find Nearest
  ResultItem *find_nearest(
      Metrics<T> *&metric,
      vector<DataItem<T> *> *&input_data,
      DataItem<T> *&query)
  {
    double distance = 999999, current_distance = 0;
    long current_id = 0;

    for (unsigned int i = 0; i < input_data->size(); i++)
    {
      current_distance = metric->get_distance(query->vector_data, input_data->at(i)->vector_data);
      if (current_distance < distance)
      {
        distance = current_distance;
        current_id = input_data->at(i)->id;
      }
    }

    ResultItem *result = new ResultItem(current_id, distance);

    return result;
  }
};

#endif