#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include "./data_item.hpp"
#include "../enums.hpp"
#include "../metrics.hpp"
#include "../data_manipulation/data_item.hpp"
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
        remove_duplicates(total_data);
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

  void remove_duplicates(vector<DataItem<T> *> *&dataset)
  {
    DataItem<T> *temp;
    for (unsigned int i = 0; i < dataset->size(); i++)
    {
      for (unsigned int j = i + 1; j < dataset->size(); j++)
      {
        if (*(dataset->at(i)->vector_data) == *(dataset->at(j)->vector_data))
        {
          temp = dataset->at(j);
          dataset->at(j) = dataset->back();
          dataset->pop_back();
          delete temp;
          j = i;
        }
      }
    }
  }

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

  // Write file
  void write_file(
      string filename,
      InitializationType init_type,
      AssignmentType assign_type,
      UpdateType update_type,
      Metrics<T> *&metrics,
      vector<Cluster<T> *> *&clusters,
      vector<DataItem<T> *> *&dataset,
      double clustering_time,
      bool is_complete)
  {
    ofstream result_file(filename, ios_base::app);
    if (result_file.is_open())
    {
      result_file << "Algorithm: "
                  << "I" << (int)init_type << "A" << (int)assign_type << "U" << (int)update_type << endl;

      result_file << "Metric: " << metrics->get_metric_type_string() << endl;

      for (unsigned int i = 0; i < clusters->size(); i++)
      {

        if (update_type == KMEANS_UPDATE)
        {
          result_file << "Cluster-" << i
                      << ": {"
                      << "size: " << clusters->at(i)->data->size() << ",\ncentroid: [";

          for (unsigned int j = 0; j < clusters->at(i)->centroid->vector_data->size(); j++)
          {
            result_file << clusters->at(i)->centroid->vector_data->at(j) << " ";
          }
          result_file << endl;
        }
        else
        {
          result_file << "Cluster-" << i
                      << ": {"
                      << "size: " << clusters->at(i)->data->size() << ", centroid: " << clusters->at(i)->centroid->id << "}" << endl;
        }
      }
      result_file << "clustering_time: " << clustering_time << endl;

      result_file << "Silhouette: [";

      double sum = 0;
      for (unsigned int i = 0; i < clusters->size(); i++)
      {
        sum += clusters->at(i)->average_silhouette;
        result_file << clusters->at(i)->average_silhouette << ", ";
      }
      result_file << sum / clusters->size();
      result_file << "]" << endl
                  << endl;

      if (is_complete)
      {
        for (unsigned int i = 0; i < clusters->size(); i++)
        {
          result_file << "Cluster-" << i << ": {";
          for (unsigned int j = 0; j < clusters->at(i)->data->size(); j++)
          {
            result_file << clusters->at(i)->data->at(j);
            if (clusters->at(i)->data->size() - 1 != j)
            {
              result_file << ", ";
            }
            else
            {
              result_file << "}" << endl
                          << endl;
              ;
            }
          }
        }
      }

      result_file.close();
    }
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
};

#endif