#ifndef H_FUNCTION_HPP
#define H_FUNCTION_HPP

#include <string>
#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include "../common/enums.hpp"

using namespace std;

template <class T>
class HFunction
{
public:
  HFunction(vector<double> *v, double t, int w, MetricType type)
      : v_(v), t_(t), w_(w), type_(type)
  {
  }

  HFunction(const HFunction &hf)
  {
    v_ = new vector<double>();
    *v_ = *hf.v_;
  }

  HFunction(HFunction &&hf)
      : v_(hf.v_)
  {
    hf.v_ = nullptr;
  }

  ~HFunction()
  {
    delete v_;
  }

  int apply_function(vector<T> *&p)
  {
    if (type_ == EUCLIDEAN)
    {
      return apply_function_with_euclidean(p);
    }
    else if (type_ == COSINE)
    {
      return apply_function_with_cosine(p);
    }
    return -1;
  }

  MetricType get_type()
  {
    return type_;
  }

private:
  vector<double> *v_;
  double t_;
  int w_;
  MetricType type_;

  int apply_function_with_euclidean(vector<T> *&p)
  {
    double dot_product = (double)inner_product(v_->begin(), v_->end(), p->begin(), 0.0);
    return (int)floor((dot_product + t_) / 2);
  }

  int apply_function_with_cosine(vector<T> *&p)
  {
    double dot_product = (double)inner_product(v_->begin(), v_->end(), p->begin(), 0.0);
    return dot_product >= 0 ? 1 : 0;
  }
};

#endif