#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../enums.hpp"
#include "./result_item.hpp"
#include "./data_item.hpp"
#include <algorithm>

using namespace std;

template <class T>
class DataManager
{
private:
  // Attributes
  bool has_errors_;
  bool is_real_;
  double R_; //radius
  double max_aporox_frac_;
  long mean_time_count_;
  double total_mean_time_;

  // Methods
  MetricType metric_type_;

  // Split And Convert
  DataItem<T> *split_and_convert(string input_string, unsigned long int id)
  {
    unsigned int position = 0;
    T converted_token = 0;
    string token;
    string delimiter = " ";
    DataItem<T> *result = new DataItem<T>();
    result->id = id;
    result->vector_data = new vector<T>();

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
        cout << "Couldn't convert file data to int" << endl;
        delete result;
        has_errors_ = true;
      }
    }

    return result;
  }

  //Is Argument?
  bool is_argument(string &first_line)
  {
    if (first_line.find("euclidean") != string::npos)
    {
      metric_type_ = EUCLIDEAN;
      return true;
    }
    else if (first_line.find("cosine") != string::npos)
    {
      metric_type_ = COSINE;
      return true;
    }
    if (first_line.find(' ') == string::npos)
    {
      try
      {
        R_ = stod(first_line);
        return true;
      }
      catch (const std::exception &e)
      {
        cout << "Couldn't convert Radius parameter from the first line" << endl;
        has_errors_ = true;
        return false;
      }
    }
    return false;
  }

public:
  // Constructor
  DataManager()
      : has_errors_(false),
        R_(0),
        max_aporox_frac_(0),
        mean_time_count_(0),
        total_mean_time_(0),
        metric_type_(EUCLIDEAN)
  {
  }

  // Read File
  vector<DataItem<T> *> *read_file(string file_name)
  {
    unsigned long int id = 0;
    DataItem<T> *result;
    string line;
    ifstream file(file_name);
    vector<DataItem<T> *> *total_data = new vector<DataItem<T> *>();

    if (file.is_open())
    {
      if (!getline(file, line))
      {
        cout << "Coulnd't read the file " << file_name << endl;
        has_errors_ = true;
        return NULL;
      }

      if (is_argument(line))
      {
        getline(file, line);
      }

      if (has_errors_)
      {
        return NULL;
      }

      do
      {
        result = split_and_convert(line, id);
        if (has_errors_ == true)
        {
          return NULL;
        }
        else
        {
          total_data->push_back(result);
        }
        id++;
      } while (getline(file, line));
    }
    file.close();

    return total_data;
  }

  // Write file
  void write_file(
      DataItem<T> *&query,
      ResultItem *&bf_result,
      vector<ResultItem *> *&alg_result,
      MetricType type,
      string result_location,
      string alg_name,
      double alg_time,
      double bf_time)
  {
    if (alg_result->size() == 0)
    {
      cout << "No result found for query: " << query->id << endl;
      cout << "The brute force result is : " << bf_result->id << " with distance: " << bf_result->distance << endl;
      return;
    }

    double frac = alg_result->at(0)->distance / bf_result->distance;

    if (frac > max_aporox_frac_)
    {
      max_aporox_frac_ = frac;
    }

    mean_time_count_++;
    total_mean_time_ += alg_time;

    sort(alg_result->begin(), alg_result->end(), compareResult);

    //string filename = "../../results/lsh/";
    string filename = result_location;

    if (type == EUCLIDEAN)
    {
      filename = filename + "euclidean_resul.txt";
    }
    else
    {
      filename = filename + "cosine_resul.txt";
    }

    ofstream result_file(filename, ios_base::app);
    if (result_file.is_open())
    {
      result_file << "Query: " << query->id << endl;
      result_file << "R-near neighbors" << endl;
      for (unsigned int i = 0; i < alg_result->size(); i++)
      {
        result_file << alg_result->at(i)->id << " (" << alg_result->at(i)->distance << ")" << endl;
      }
      result_file << "Nearest neighbor: " << alg_result->at(0)->id << endl;
      result_file << "distance" + alg_name + ": " << alg_result->at(0)->distance << endl;
      result_file << "distanceTrue: " << bf_result->distance << endl;
      result_file << "t" + alg_name + ": " << alg_time << endl;
      result_file << "tTrue: " << bf_time << endl
                  << endl;

      result_file.close();
    }
  }
  // Write file without bf
  void write_file(
      DataItem<T> *&query,

      vector<ResultItem *> *&alg_result,
      MetricType type,
      string result_location,
      string alg_name,
      double alg_time)
  {
    if (alg_result->size() == 0)
    {
      cout << "No result found for query: " << query->id << endl;
      return;
    }

    mean_time_count_++;
    total_mean_time_ += alg_time;

    sort(alg_result->begin(), alg_result->end(), compareResult);

    //string filename = "../../results/lsh/";
    string filename = result_location;

    if (type == EUCLIDEAN)
    {
      filename = filename + "euclidean_resul.txt";
    }
    else
    {
      filename = filename + "cosine_resul.txt";
    }

    ofstream result_file(filename, ios_base::app);
    if (result_file.is_open())
    {
      result_file << "Query: " << query->id << endl;
      result_file << "R-near neighbors" << endl;
      for (unsigned int i = 0; i < alg_result->size(); i++)
      {
        result_file << alg_result->at(i)->id << " (" << alg_result->at(i)->distance << ")" << endl;
      }
      result_file << "Nearest neighbor: " << alg_result->at(0)->id << endl;
      result_file << "distance" + alg_name + ": " << alg_result->at(0)->distance << endl;
      result_file << "t" + alg_name + ": " << alg_time << endl
                  << endl;

      result_file.close();
    }
  }

  // Has Errors?
  bool has_errors()
  {
    return has_errors_;
  }

  // Get Metric Type
  MetricType get_metric_type()
  {
    return metric_type_;
  }

  // Get Radius
  double get_radius()
  {
    return R_;
  }

  void print_max_aprox_fraction()
  {
    cout << "The maximum approximate fraction is : " << max_aporox_frac_ << endl;
  }

  void print_mean_time()
  {
    cout << "The mean time is : " << total_mean_time_ / mean_time_count_ << endl;
  }
};

#endif