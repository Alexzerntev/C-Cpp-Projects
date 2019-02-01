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
  vector<bool> *is_known;
  double with_distance;
  double lsh_distance;
  int current_cluster;
  double silhouette;
  double average_score;
  bool is_in_use;

  // Constructors
  DataItem()
      : is_dirty(false), is_known(nullptr), with_distance(-1), current_cluster(-1), silhouette(-2), average_score(0), is_in_use(true)
  {
  }

  // Copy constructor
  DataItem(const DataItem &data_item)
  {
    id = data_item->id;
    vector_data = new vector<T>();
    *vector_data = *data_item.vector_data;
    is_known = new vector<bool>();
    *is_known = *data_item.is_known;
  }

  // Move constructor
  DataItem(DataItem &&data_item)
      : vector_data(data_item.vector_data)
  {
    data_item.vector_data = nullptr;
    data_item.is_known = nullptr;
  }

  DataItem(DataItem *&data_item)
  {
    id = -1;
    vector_data = new vector<T>();

    if (data_item->is_known != nullptr)
    {
      is_known = new vector<bool>();
    }

    for (unsigned int i = 0; i < data_item->vector_data->size(); i++)
    {
      vector_data->push_back(data_item->vector_data->at(i));

      if (data_item->is_known != nullptr)
      {
        is_known->push_back(data_item->is_known->at(i));
      }
    }
  }

  ~DataItem()
  {
    delete vector_data;
    delete is_known;
  }

  void normalize()
  {
    double sum;
    int count = 0;
    for (unsigned int i = 0; i < vector_data->size(); i++)
    {

      if (is_known->at(i) == true)
      {
        count++;
        sum += vector_data->at(i);
      }
    }

    if (count == 0)
    {
      count = 1;
    }

    sum = sum / count;
    average_score = sum;

    for (unsigned int i = 0; i < vector_data->size(); i++)
    {
      vector_data->at(i) -= sum;
    }
  }

  vector<string> get_recomended(vector<string> *coins, unsigned int limit)
  {
    vector<string> result;
    vector<pair<int, double>> pairs;
    int to_delete = -1;

    for (unsigned int i = 0; i < vector_data->size(); i++)
    {
      if (is_known->at(i) == true)
      {
        continue;
      }

      if (pairs.size() < limit)
      {
        pairs.push_back(pair<int, double>(i, vector_data->at(i)));
        continue;
      }

      for (unsigned int j = 0; j < pairs.size(); j++)
      {
        if (vector_data->at(i) > pairs.at(j).second && to_delete == -1)
        {
          to_delete = j;
        }
        if (to_delete != -1)
        {
          if (pairs.at(j).second < pairs.at(to_delete).second)
          {
            to_delete = j;
          }
        }
      }
      if (to_delete != -1)
      {
        pairs.erase(pairs.begin() + to_delete);
        pairs.push_back(pair<int, double>(i, vector_data->at(i)));
        to_delete = -1;
      }
    }

    for (unsigned int i = 0; i < pairs.size(); i++)
    {
      result.push_back(coins->at(pairs.at(i).first));
    }
    return result;
  }
};

template <class T>
bool compareResult(DataItem<T> *a, DataItem<T> *b) { return (a->lsh_distance < b->lsh_distance); }

#endif
