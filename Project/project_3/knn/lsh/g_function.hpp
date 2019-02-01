#ifndef G_FUNCTION_HPP
#define G_FUNCTION_HPP

#include <string>
#include <iostream>
#include <vector>
#include "../knn_common/h_function.hpp"

using namespace std;

template <class T>
class GFunction
{
public:
  vector<HFunction<T> *> *g;
  GFunction(vector<HFunction<T> *> *hi)
      : g(hi)
  {
  }

  GFunction(const GFunction &gf)
  {
    g = new vector<HFunction<T> *>();
    *g = *gf.g;
  }

  GFunction(GFunction &&gf)
      : g(gf.g)
  {
    gf.g = nullptr;
  }

  ~GFunction()
  {
    HFunction<T> *temp;
    while (g->size() > 0)
    {
      temp = g->at(0);
      g->erase(g->begin());
      delete temp;
    }
    delete g;
  }

  string apply_function(vector<T> *&p)
  {
    string result = "";
    for (unsigned int i = 0; i < g->size(); i++)
    {
      result = result + to_string(g->at(i)->apply_function(p));
    }
    return result;
  }

  MetricType get_type()
  {
    return type_;
  }

private:
  MetricType type_;
};

#endif