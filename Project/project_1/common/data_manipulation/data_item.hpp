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

  // Constructors
  DataItem() {}

  DataItem(unsigned long int id, vector<T> vec)
      : id(id), vector_data(vec)
  {
  }

  // Copy constructor
  DataItem(const DataItem &data_item)
  {
    vector_data = new vector<T>();
    *vector_data = *data_item.vector_data;
  }

  // Move constructor
  DataItem(DataItem &&data_item)
      : vector_data(data_item.vector_data)
  {
    data_item.vector_data = nullptr;
  }

  ~DataItem()
  {
    delete vector_data;
  }
};

#endif
