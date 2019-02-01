#ifndef DATA_ITEM_HPP
#define DATA_ITEM_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../enums.hpp"

using namespace std;

template <class T>
class DataItem
{
private:
public:
  unsigned long int id;
  vector<T> *vector_data;
  bool is_dirty;
  double with_distance;
  int current_cluster;
  double silhouette;

  // Constructors
  DataItem()
      : is_dirty(false), with_distance(-1), current_cluster(-1), silhouette(-2)
  {
  }

  // Copy constructor
  DataItem(const DataItem &data_item)
  {
    id = data_item->id;
    vector_data = new vector<T>();
    *vector_data = *data_item.vector_data;
  }

  // Move constructor
  DataItem(DataItem &&data_item)
      : vector_data(data_item.vector_data)
  {
    data_item.vector_data = nullptr;
  }

  DataItem(DataItem *&data_item)
  {
    id = -1;
    vector_data = new vector<T>();

    for (unsigned int i = 0; i < data_item->vector_data->size(); i++)
    {
      vector_data->push_back(data_item->vector_data->at(i));
    }
  }

  ~DataItem()
  {
    delete vector_data;
  }
};

#endif
