#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <string>
#include <iostream>
#include <vector>
#include "../common/data_manipulation/data_item.hpp"

using namespace std;

template <class T>
class Cluster
{
private:
public:
  int index;
  double average_silhouette;
  DataItem<T> *centroid;
  vector<DataItem<T> *> *data;

  Cluster(int _index, DataItem<T> *center)
  {
    index = _index;
    centroid = center;
    data = new vector<DataItem<T> *>();
  }

  ~Cluster()
  {
    delete data;
  }
};

#endif
