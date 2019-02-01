#ifndef METRICS_HPP
#define METRICS_HPP

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <math.h>
#include <functional>
#include "./enums.hpp"

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
      double distance = 0;
      vector<double> auxiliary;
      transform(point1->begin(), point1->end(), point2->begin(), back_inserter(auxiliary), //
                [](T element1, T element2) { return pow((element1 - element2), 2); });
      auxiliary.shrink_to_fit();
      distance = sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));

      if (isnan(distance) || isinf(distance))
      {
        return 99999;
      }

      return distance;
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
    // try
    // {
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
    for (unsigned int i = 0; i < point1->size(); ++i)
    {
      dot += point1->at(i) * point2->at(i);
      denom_a += point1->at(i) * point1->at(i);
      denom_b += point2->at(i) * point2->at(i);
    }
    double res = 1 - (dot / (sqrt(denom_a) * sqrt(denom_b)));

    if (isnan(res) || isinf(res))
    {
      return 99999;
    }

    return res;
    // }
    // catch (const std::exception &e)
    // {
    //   // cout << point1->size() << endl;

    //   // cout << point2->size() << endl;
    //   std::cerr << e.what() << '\n';
    // }
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

  string get_metric_type_string()
  {
    if (type_ == EUCLIDEAN)
    {
      return "Euclidean";
    }
    else
    {
      return "Cosine";
    }
  }
};

#endif