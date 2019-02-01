#ifndef METRICS_HPP
#define METRICS_HPP

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include "./enums.hpp"
#include <numeric>
#include <algorithm>
#include <iterator>
#include <functional>

using namespace std;

template <class T>
class Metrics
{
private:
  bool has_errors_;
  MetricType type_;

  // Get Euclidean Distance
  double get_euclidean_distance(vector<T> *&point1, vector<T> *&point2)
  {
    try
    {
      vector<double> auxiliary;
      transform(point1->begin(), point1->end(), point2->begin(), back_inserter(auxiliary), //
                [](T element1, T element2) { return pow((element1 - element2), 2); });
      auxiliary.shrink_to_fit();

      return sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0));
    }
    catch (const std::exception &e)
    {
      cout << "Data and query have different dimentions" << endl;
      return -1;
    }
  }

  // Get Cosine Distance
  double get_cosine_distance(vector<T> *&point1, vector<T> *&point2)
  {
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
    for (unsigned int i = 0; i < point1->size(); ++i)
    {
      dot += point1->at(i) * point2->at(i);
      denom_a += point1->at(i) * point1->at(i);
      denom_b += point2->at(i) * point2->at(i);
    }
    return 1 - (dot / (sqrt(denom_a) * sqrt(denom_b)));
  }

public:
  // Constructor
  Metrics(MetricType type)
  {
    type_ = type;
  }

  // Get Distance
  double get_distance(vector<T> *&point1, vector<T> *&point2)
  {
    switch (type_)
    {
    case EUCLIDEAN:
      return get_euclidean_distance(point1, point2);

    case COSINE:
      return get_cosine_distance(point1, point2);

    default:
      return -1;
    }
  }

  int get_hamming_distance(string str1, string str2)
  {
    int count = 0;

    for (unsigned int i = 0; i < str1.size(); i++)
    {
      if (str1[i] != str2[i])
      {
        count++;
      }
    }
    return count;
  }

  MetricType get_metric_type()
  {
    return type_;
  }
};

#endif