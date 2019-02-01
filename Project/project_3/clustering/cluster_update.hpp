#ifndef CLUSTER_UPDATE_HPP
#define CLUSTER_UPDATE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include "./cluster.hpp"
#include "./cluster_helpers.hpp"
#include "./cluster_insert.hpp"
#include "../common/random_generator.hpp"
#include "../common/metrics.hpp"
#include "../common/enums.hpp"
#include "../common/argument_initialize.hpp"
#include "../common/data_manipulation/data_manager.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../knn/lsh/lsh_execution.hpp"
#include "../knn/cube/cube_execution.hpp"

using namespace std;

template <class T>
class ClusterUpdate
{
private:
  UpdateType type_;

public:
  void clustering_execute(
      vector<DataItem<T> *> *&dataset,
      vector<Cluster<T> *> *&clusters,
      DataManager<T> *&data_manager,
      Metrics<T> *&metrics,
      ClusterInsert<T> *&cluster_insert)
  {

    switch (type_)
    {
    case KMEANS_UPDATE:
      update_with_kmeans(dataset, clusters, data_manager, metrics, cluster_insert);
      break;

    case PAM_UPDATE:
      update_with_pam(dataset, clusters, data_manager, metrics, cluster_insert);
      break;

    default:
      break;
    }
  }

  //// REMEMBER TO CLEAR THE MEMORY OF THE NEW CENTROID!
  void update_with_kmeans(
      vector<DataItem<T> *> *&dataset,
      vector<Cluster<T> *> *&clusters,
      DataManager<T> *&data_manager,
      Metrics<T> *&metrics,
      ClusterInsert<T> *&cluster_insert)
  {
    bool should_remain = true;
    for (unsigned int i = 0; i < clusters->size(); i++)
    {
      DataItem<T> *new_centroid = new DataItem<T>(clusters->at(i)->centroid);
      new_centroid->id = 0;
      clusters->at(i)->centroid = new_centroid;
    }

    vector<T> *current_mean = new vector<T>();

    int counter = 0;
    while (should_remain)
    {

      counter++;
      should_remain = false;

      cluster_insert->insert_to_cluster(dataset, clusters, data_manager, metrics);

      for (unsigned int i = 0; i < clusters->size(); i++)
      {
        current_mean->clear();
        for (unsigned int k = 0; k < clusters->at(i)->centroid->vector_data->size(); k++)
        {
          current_mean->push_back(clusters->at(i)->centroid->vector_data->at(k));
        }

        set_next_mean(clusters->at(i));
        if (metrics->get_distance(current_mean, clusters->at(i)->centroid->vector_data) > 0.0000000001 && counter < 100)
        {
          should_remain = true;
        }
      }
    }
    delete current_mean;
    return;
  }

  void set_next_mean(Cluster<T> *&cluster)
  {
    fill(cluster->centroid->vector_data->begin(), cluster->centroid->vector_data->end(), 0);

    for (unsigned int i = 0; i < cluster->data->size(); i++)
    {
      for (unsigned int j = 0; j < cluster->centroid->vector_data->size(); j++)
      {
        cluster->centroid->vector_data->at(j) += cluster->data->at(i)->vector_data->at(j);
      }
    }

    transform(
        cluster->centroid->vector_data->begin(),
        cluster->centroid->vector_data->end(),
        cluster->centroid->vector_data->begin(),
        bind(divides<T>(), placeholders::_1, cluster->data->size()));
  }

  void update_with_pam(
      vector<DataItem<T> *> *&dataset,
      vector<Cluster<T> *> *&clusters,
      DataManager<T> *&data_manager,
      Metrics<T> *&metrics,
      ClusterInsert<T> *&cluster_insert)
  {

    // Initialization
    bool continue_condition = true;
    vector<unsigned long int> best_indexes;
    vector<double> best_costs;
    for (unsigned int i = 0; i < clusters->size(); i++)
    {
      best_indexes.push_back(get_index_of_id(dataset, clusters->at(i)->centroid->id));
      best_costs.push_back(9999999);
    }

    while (continue_condition)
    {
      continue_condition = false;
      for (unsigned int i = 0; i < clusters->size(); i++)
      {
        clusters->at(i)->centroid->is_dirty = false;
        clusters->at(i)->centroid->current_cluster = -1;
        clusters->at(i)->centroid = dataset->at(best_indexes.at(i));
        clusters->at(i)->centroid->is_dirty = true;
        clusters->at(i)->centroid->current_cluster = i;
      }

      cluster_insert->insert_to_cluster(dataset, clusters, data_manager, metrics);

      double cost = 0;

      for (unsigned int i = 0; i < clusters->size(); i++)
      {
        for (unsigned int j = 0; j < clusters->at(i)->data->size(); j++)
        {
          clusters->at(i)->centroid = clusters->at(i)->data->at(j);
          cost = get_objective_cost(clusters, i, metrics);
          if (cost < best_costs.at(i))
          {
            best_indexes.at(i) = get_index_of_id(dataset, clusters->at(i)->centroid->id);
            best_costs.at(i) = cost;
            continue_condition = true;
          }
        }
      }
    }
    cluster_insert->insert_to_cluster(dataset, clusters, metrics);
  }

  double get_objective_cost(vector<Cluster<T> *> *&clusters, int index, Metrics<T> *&metric)
  {
    double sum = 0;

    for (unsigned int i = 0; i < clusters->at(index)->data->size(); i++)
    {
      if (!is_center(clusters, clusters->at(index)->data->at(i)->id))
      {
        sum += metric->get_distance(clusters->at(index)->centroid->vector_data, clusters->at(index)->data->at(i)->vector_data);
      }
    }
    return sum;
  }

  int get_index_of_id(vector<DataItem<T> *> *&dataset, unsigned long int id)
  {
    for (unsigned int i = 0; i < dataset->size(); i++)
    {
      if (dataset->at(i)->id == id)
      {
        return i;
      }
    }
    return 0;
  }

  ClusterUpdate(UpdateType type)
  {
    type_ = type;
  }

  ~ClusterUpdate() {}
};

#endif
