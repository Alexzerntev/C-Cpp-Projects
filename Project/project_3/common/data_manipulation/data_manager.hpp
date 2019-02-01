#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <map>
#include "./data_item.hpp"
#include "../enums.hpp"
#include "../metrics.hpp"
#include "../data_manipulation/data_item.hpp"
#include "../data_manipulation/user_data_item.hpp"
#include "../../clustering/cluster.hpp"

using namespace std;

template <class T>
class DataManager
{
private:
  int number_of_clusters_;
  int number_of_hash_functions_;
  int number_of_hash_tables_;
  int probes_;
  unsigned int M_;

public:
  // Constructor
  DataManager()
      : number_of_clusters_(4), number_of_hash_functions_(4), number_of_hash_tables_(5), probes_(2), M_(1000)
  {
  }

  DataManager(int probes)
      : number_of_clusters_(4), number_of_hash_functions_(4), number_of_hash_tables_(5), probes_(probes), M_(1000)
  {
  }

  // Project 1-2 stuff ==============================================================================================
  DataItem<T> *split_and_convert(string input_string)
  {
    unsigned int position = 0;
    T converted_token = 0;
    string token;
    string delimiter = ",";
    DataItem<T> *result = new DataItem<T>();
    result->vector_data = new vector<T>();
    bool is_first = true;

    while ((position = input_string.find(delimiter)) != string::npos)
    {
      token = input_string.substr(0, position);
      input_string.erase(0, position + delimiter.length());

      if (token == "" || token == "\r")
      {
        break;
      }

      try
      {
        if (is_first)
        {
          result->id = stoi(token);
          is_first = false;
          continue;
        }
        if (is_same<T, int>::value)
        {
          converted_token = stoi(token);
        }
        else
        {
          converted_token = stod(token);
        }
        result->vector_data->push_back(converted_token);
      }
      catch (const std::exception &e)
      {
        delete result;
        throw invalid_argument("Couldn't convert file data to digits ");
      }
    }
    return result;
  }

  // Read File
  vector<DataItem<T> *> *read_data(string file_name)
  {
    DataItem<T> *result;
    string line;
    ifstream file(file_name);
    vector<DataItem<T> *> *total_data = new vector<DataItem<T> *>();
    try
    {
      if (file.is_open())
      {
        while (getline(file, line))
        {
          result = split_and_convert(line);
          total_data->push_back(result);
        }
      }
      else
      {
        throw "Coudlnt Open The Data File";
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
    return total_data;
  }

  // Read config
  void read_config(string file_name)
  {
    ifstream file(file_name);
    string line;
    try
    {
      if (file.is_open())
      {
        while (getline(file, line))
        {
          if (line.find("number_of_clusters:") != string::npos)
          {
            line.erase(line.begin(), line.begin() + 19);
            number_of_clusters_ = stoi(line);
          }
          else if (line.find("number_of_hash_functions:") != string::npos)
          {
            line.erase(line.begin(), line.begin() + 25);
            number_of_hash_functions_ = stoi(line);
          }
          else if (line.find("number_of_hash_tables:") != string::npos)
          {
            line.erase(line.begin(), line.begin() + 22);
            number_of_hash_tables_ = stoi(line);
          }
          else
          {
            throw "Wrong settings format";
          }
        }
      }
      else
      {
        throw "Couldn't open file ";
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
    file.close();
    return;
  }

  int get_number_of_clusters()
  {
    return number_of_clusters_;
  }

  int get_number_of_hash_functions()
  {
    return number_of_hash_functions_;
  }

  int get_number_of_hash_tables()
  {
    return number_of_hash_tables_;
  }
  int get_probes()
  {
    return probes_;
  }
  unsigned int get_M()
  {
    return M_;
  }

  // Project 3 stuff
  map<string, double> *create_sentiment_dictionary(string file_name)
  {
    unsigned int position = 0;
    string delimiter = "\t";
    double value = 0;
    int index = 0;
    map<string, double> *dictionary = new map<string, double>();
    string line, key, token;

    ifstream file(file_name);
    try
    {
      if (file.is_open())
      {
        while (getline(file, line))
        {
          while ((position = line.find(delimiter)) != string::npos)
          {
            token = line.substr(0, position);
            line.erase(0, position + delimiter.length());

            if (token == "" || token == "\r")
            {
              break;
            }

            try
            {
              if (index == 0)
              {
                key = token;
                index++;
                continue;
              }
              if (index == 1)
              {
                value = stod(token);
                index++;
                continue;
              }
            }
            catch (const std::exception &e)
            {
              delete dictionary;
              throw invalid_argument("Couldn't convert file data to digits ");
            }
          }
          dictionary->insert(pair<string, double>(key, value));
          index = 0;
        }
      }
      else
      {
        throw "Couldn't Open The Data File";
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
    return dictionary;
  }

  vector<UserDataItem *> *read_user_data(string file_name)
  {
    UserDataItem *result;
    string line;
    ifstream file(file_name);
    vector<UserDataItem *> *total_data = new vector<UserDataItem *>();
    try
    {
      if (file.is_open())
      {
        while (getline(file, line))
        {
          result = split_and_convert_user_data(line);
          total_data->push_back(result);
        }
      }
      else
      {
        throw "Couldn't Open The Data File";
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
    return total_data;
  }

  UserDataItem *split_and_convert_user_data(string input_string)
  {
    unsigned int position = 0;
    string token;
    string delimiter = "\t";
    UserDataItem *result = new UserDataItem();
    result->tweet_data = new vector<string>();
    int index = 0;

    while ((position = input_string.find(delimiter)) != string::npos)
    {
      token = input_string.substr(0, position);
      input_string.erase(0, position + delimiter.length());

      if (token == "" || token == "\r")
      {
        break;
      }

      try
      {
        if (index == 0)
        {
          result->user_id = stoi(token);
          index++;
          continue;
        }

        if (index == 1)
        {
          result->tweet_id = stoi(token);
          index++;
          continue;
        }
        result->tweet_data->push_back(token);
      }
      catch (const std::exception &e)
      {
        delete result;
        throw invalid_argument("Couldn't convert file data to digits ");
      }
    }
    return result;
  }

  map<string, string> *create_coin_dictionary(string file_name)
  {
    unsigned int position = 0;
    string delimiter = "\t";
    int index = 0;
    map<string, string> *dictionary = new map<string, string>();
    string line, key, token, value;

    ifstream file(file_name);
    try
    {
      if (file.is_open())
      {
        while (getline(file, line))
        {
          while ((position = line.find(delimiter)) != string::npos)
          {
            token = line.substr(0, position);
            line.erase(0, position + delimiter.length());

            if (token == "" || token == "\r")
            {
              break;
            }

            if (token.at(token.size() - 1) == '\r')
            {
              token.erase(token.size() - 1);
            }

            try
            {
              if (index == 0)
              {
                value = token;
                index++;
              }
              key = token;
              dictionary->insert(pair<string, string>(key, value));
            }
            catch (const std::exception &e)
            {
              delete dictionary;
              throw invalid_argument("Couldn't convert file data to digits ");
            }
          }
          index = 0;
        }
      }
      else
      {
        throw "Couldn't Open The Data File";
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
    return dictionary;
  }

  vector<string> *get_distinct_coin_names(string file_name)
  {
    unsigned int position = 0;
    string delimiter = "\t";
    vector<string> *result = new vector<string>();
    string line, token;

    ifstream file(file_name);
    try
    {
      if (file.is_open())
      {
        while (getline(file, line))
        {
          while ((position = line.find(delimiter)) != string::npos)
          {
            token = line.substr(0, position);
            line.erase(0, position + delimiter.length());

            if (token == "" || token == "\r")
            {
              break;
            }
            result->push_back(token);
            break;
          }
        }
      }
      else
      {
        throw "Couldn't Open The Data File";
        delete result;
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
    return result;
  }

  vector<DataItem<T> *> *create_user_profiles(
      vector<UserDataItem *> *data,
      vector<string> *distinct_coin_names)
  {
    vector<DataItem<T> *> *result = new vector<DataItem<T> *>();
    DataItem<T> *temp;
    bool found;
    for (unsigned int i = 0; i < data->size(); i++)
    {
      found = false;
      for (unsigned int j = 0; j < result->size(); j++)
      {
        if ((long unsigned int)data->at(i)->user_id == result->at(j)->id)
        {
          found = true;
          break;
        }
      }
      if (found == false)
      {
        temp = new DataItem<T>();
        temp->id = data->at(i)->user_id;
        temp->vector_data = new vector<T>(distinct_coin_names->size(), 0);
        temp->is_known = new vector<bool>(distinct_coin_names->size(), false);
        result->push_back(temp);
      }
    }
    return result;
  }

  void write_algorithm(string algorithm, string filename)
  {
    ofstream result_file(filename, ios_base::app);
    if (result_file.is_open())
    {
      result_file << algorithm << endl;
    }
    result_file.close();
  }

  // Write file
  void write_file_partial(unsigned long int user_id, vector<string> &best_coins, string filename)
  {
    ofstream result_file(filename, ios_base::app);
    if (result_file.is_open())
    {
      result_file << user_id << " ";
      for (unsigned int i = 0; i < best_coins.size(); i++)
      {
        result_file << best_coins.at(i) << " ";
      }
      result_file << endl;
    }
    result_file.close();
  }

  void write_file_time(double execution_time, string filename)
  {
    ofstream result_file(filename, ios_base::app);
    if (result_file.is_open())
    {
      result_file << "Execution Time: " << execution_time << endl;
    }
    result_file.close();
  }

  void write_file_evaluation(double mae, string filename, bool is_lsh)
  {
    ofstream result_file(filename, ios_base::app);
    if (result_file.is_open())
    {
      if (is_lsh)
      {
        result_file << "Cosine LSH Recomendation MAE: " << mae << endl;
      }
      else
      {
        result_file << "Clustering Recomendation MAE: " << mae << endl;
      }
    }
    result_file.close();
  }
};

#endif