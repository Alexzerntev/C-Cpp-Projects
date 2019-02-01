#ifndef RECOMENDATION_HELPERS_HPP
#define RECOMENDATION_HELPERS_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>

#include "../clustering/cluster.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../common/data_manipulation/user_data_item.hpp"

using namespace std;

template <class T>
int get_user_index(vector<DataItem<T> *> *data, unsigned long int id)
{
  for (unsigned int i = 0; i < data->size(); i++)
  {
    if (data->at(i)->id == id)
    {
      return i;
    }
  }
  return -1;
}

template <class T>
void normalize_data(vector<DataItem<T> *> *data)
{
  for (unsigned int i = 0; i < data->size(); i++)
  {
    data->at(i)->normalize();
  }
}

template <class T>
vector<UserDataItem *> *generate_dataset_from_cluster(vector<Cluster<T> *> *clusters, vector<UserDataItem *> *user_data)
{
  vector<UserDataItem *> *result = new vector<UserDataItem *>();
  bool found = false;
  for (unsigned int i = 0; i < clusters->size(); i++)
  {
    for (unsigned int j = 0; j < clusters->at(i)->data->size(); j++)
    {
      UserDataItem *user = new UserDataItem();
      user->user_id = -1 * i;
      user->tweet_id = clusters->at(i)->data->at(j)->id;
      user->tweet_data = new vector<string>();

      for (unsigned int k = 0; k < user_data->size(); k++)
      {
        if (user->tweet_id == user_data->at(k)->tweet_id)
        {
          found = true;
          for (unsigned int z = 0; z < user_data->at(k)->tweet_data->size(); z++)
          {
            user->tweet_data->push_back(user_data->at(k)->tweet_data->at(z));
          }
        }
      }
      if (found == true)
      {
        result->push_back(user);
        found = false;
      }
      else
      {
        delete user;
      }
    }
  }
  return result;
}

template <class T>
DataItem<T> *execute_recomendation(DataItem<T> *user, vector<DataItem<T> *> *data, bool is_clustering)
{
  double normilizing_factor = 0;
  DataItem<T> *result = new DataItem<T>();
  result->vector_data = new vector<double>();
  result->is_known = new vector<bool>();

  result->average_score = user->average_score;
  result->id = user->id;
  result->lsh_distance = user->lsh_distance;

  for (unsigned int i = 0; i < user->is_known->size(); i++)
  {
    result->is_known->push_back(user->is_known->at(i));
    result->vector_data->push_back(user->vector_data->at(i));
  }

  for (unsigned int i = 0; i < data->size(); i++)
  {
    if (data->at(i)->id == user->id)
    {
      continue;
    }

    if (is_clustering)
    {
      normilizing_factor += data->at(i)->with_distance;
    }
    else
    {
      normilizing_factor += data->at(i)->lsh_distance;
    }
  }

  // normilizing_factor = 1 / normilizing_factor;

  double sum;
  for (unsigned int i = 0; i < result->vector_data->size(); i++)
  {
    sum = 0;
    if (result->is_known->at(i) == false)
    {

      if (is_clustering)
      {
        for (unsigned int j = 0; j < data->size(); j++)
        {
          sum = sum + (data->at(j)->with_distance * (data->at(j)->vector_data->at(i)));
        }
      }
      else
      {
        for (unsigned int j = 0; j < data->size(); j++)
        {
          sum = sum + (data->at(j)->lsh_distance * (data->at(j)->vector_data->at(i)));
        }
      }
      if (normilizing_factor == 0)
      {
        normilizing_factor = 1;
      }
      result->vector_data->at(i) = ((1 / abs(normilizing_factor)) * sum);
    }
  }
  return result;
}

int get_coin_index(vector<string> *coins, string name);

#endif