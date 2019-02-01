#ifndef SENTIMENT_HPP
#define SENTIMENT_HPP

#include <string>
#include <iostream>
#include <map>
#include <math.h>

#include "../common/data_manipulation/user_data_item.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../recomendation/recomendation_helpers.hpp"

using namespace std;

template <class T>
class Sentiment
{
private:
public:
  void assign_sentiment(
      vector<UserDataItem *> *data,
      map<string, double> *dictionary)
  {
    double sum;
    double alpha = 15;
    for (unsigned int i = 0; i < data->size(); i++)
    {
      sum = 0;
      for (unsigned int j = 0; j < data->at(i)->tweet_data->size(); j++)
      {
        try
        {
          sum += dictionary->at(data->at(i)->tweet_data->at(j));
        }
        catch (const exception &e)
        {
        }
      }
      data->at(i)->sentiment = sum / sqrt((pow(sum, 2) + alpha));
    }
  }

  void fill_data_items(
      vector<DataItem<T> *> *data,
      vector<string> *coins,
      vector<UserDataItem *> *user_data,
      map<string, string> *coin_map)
  {
    int coin_index = 0;
    int user_index = 0;
    string coin;
    for (unsigned int i = 0; i < user_data->size(); i++)
    {
      for (unsigned int j = 0; j < user_data->at(i)->tweet_data->size(); j++)
      {
        try
        {
          coin = coin_map->at(user_data->at(i)->tweet_data->at(j));
          coin_index = get_coin_index(coins, coin);
          user_index = get_user_index(data, user_data->at(i)->user_id);
          data->at(user_index)->vector_data->at(coin_index) += user_data->at(i)->sentiment;
          data->at(user_index)->is_known->at(coin_index) = true;
        }
        catch (const std::exception &e)
        {
        }
      }
    }
    clean_zeros(data);
  }

  void clean_zeros(
      vector<DataItem<T> *> *data)
  {
    bool found = false;
    for (unsigned int i = 0; i < data->size(); i++)
    {
      for (unsigned int j = 0; j < data->at(i)->vector_data->size(); j++)
      {
        if (data->at(i)->vector_data->at(j) != 0 || data->at(i)->is_known->at(j) != false)
        {
          found = true;
          break;
        }
      }
      if (found == false)
      {
        data->at(i) = data->back();
        data->pop_back();
      }
      else
      {
        found = false;
      }
    }
  }
};

#endif