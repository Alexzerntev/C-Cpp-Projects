#ifndef CLUSTER_INSERT_HPP
#define CLUSTER_INSERT_HPP

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "./cluster.hpp"
#include "./cluster_helpers.hpp"
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
class ClusterInsert
{
  private:
    AssignmentType type_;
    LshExecution<T> *lsh_execution_;
    CubeExecution<T> *cube_execution_;

  public:
    void insert_to_cluster(
        vector<DataItem<double> *> *&dataset,
        vector<Cluster<double> *> *&clusters,
        DataManager<T> *&data_manager,
        Metrics<T> *&metrics)
    {

        switch (type_)
        {
        case LLOYDS_ASSIGNMENT:
            insert_with_simple_lloyed(dataset, clusters, metrics);
            break;

        case LSH_ASSIGNMENT:
            insert_with_lsh(dataset, clusters, metrics);
            break;

        case CUBE_ASSIGNMENT:
            insert_with_cube(dataset, clusters, data_manager, metrics);
            break;

        default:
            break;
        }
    }

    void insert_to_cluster(
        vector<DataItem<double> *> *&dataset,
        vector<Cluster<double> *> *&clusters,
        Metrics<T> *&metrics)
    {
        insert_with_simple_lloyed(dataset, clusters, metrics);
    }

    // Simple Lloyd
    void insert_with_simple_lloyed(
        vector<DataItem<T> *> *&dataset,
        vector<Cluster<T> *> *&clusters,
        Metrics<T> *&metrics)
    {
        int min_distance_cluster_index = 0;
        double current_distance = 0;
        double min_distance = 9999999;

        // First clean
        if (type_ == LLOYDS_ASSIGNMENT)
        {
            reset_dataset(clusters, dataset);
        }

        // Then Assign
        for (unsigned int i = 0; i < dataset->size(); i++)
        {
            if (is_center(clusters, dataset->at(i)->id))
            {
                continue;
            }

            min_distance_cluster_index = 0;
            current_distance = 0;
            min_distance = 9999999;
            for (unsigned int j = 0; j < clusters->size(); j++)
            {
                current_distance = metrics->get_distance(clusters->at(j)->centroid->vector_data, dataset->at(i)->vector_data);
                if (min_distance > current_distance)
                {
                    min_distance = current_distance;
                    min_distance_cluster_index = j;
                }
            }
            dataset->at(i)->is_dirty = true;
            dataset->at(i)->with_distance = min_distance;
            dataset->at(i)->current_cluster = min_distance_cluster_index;
            clusters->at(min_distance_cluster_index)->data->push_back(dataset->at(i));
        }
        return;
    }

    // LSH
    void insert_with_lsh(
        vector<DataItem<T> *> *&dataset,
        vector<Cluster<T> *> *&clusters,
        Metrics<T> *&metrics)
    {
        int points_returned = -1;
        bool is_first_time = true;
        double current_radius = 0;

        reset_dataset(clusters, dataset);

        current_radius = get_min_distance_between_clusters(clusters, metrics) / 2;

        vector<bool> has_retun_points(clusters->size(), true);

        while (check_if_no_return_points(has_retun_points))
        {
            for (unsigned int i = 0; i < clusters->size(); i++)
            {
                if (has_retun_points.at(i) == true)
                {
                    points_returned = lsh_execution_->find_nearest(
                        metrics,
                        clusters->at(i)->centroid,
                        current_radius, i,
                        clusters,
                        is_first_time);

                    if (points_returned == 0)
                    {
                        has_retun_points[i] = false;
                    }
                }
            }
            is_first_time = false;
            current_radius = current_radius * 2;
        }
        insert_with_simple_lloyed(dataset, clusters, metrics);
    }

    // Hypercube
    void insert_with_cube(
        vector<DataItem<T> *> *&dataset,
        vector<Cluster<T> *> *&clusters,
        DataManager<T> *&data_manager,
        Metrics<T> *&metrics)
    {
        int points_returned = true;
        bool is_first_time = true;
        double current_radius = 0;

        current_radius = get_min_distance_between_clusters(clusters, metrics) / 2;

        vector<bool> has_retun_points(clusters->size(), true);

        while (check_if_no_return_points(has_retun_points))
        {
            for (unsigned int i = 0; i < clusters->size(); i++)
            {
                if (has_retun_points.at(i) == true)
                {
                    points_returned = cube_execution_->find_nearest(
                        metrics,
                        clusters->at(i)->centroid,
                        current_radius, i,
                        clusters,
                        is_first_time,
                        data_manager);

                    if (points_returned == 0)
                    {
                        has_retun_points[i] = false;
                    }
                }
            }
            is_first_time = false;
            current_radius = current_radius * 2;
        }
        insert_with_simple_lloyed(dataset, clusters, metrics);
    }

    bool check_if_no_return_points(vector<bool> &cluster_state)
    {
        if (all_of(cluster_state.begin(), cluster_state.end(), [](bool i) { return i == false; }))
        {
            return true;
        }
        return false;
    }

    void reset_dataset(vector<Cluster<T> *> *&clusters, vector<DataItem<T> *> *&dataset)
    {
        for (unsigned int i = 0; i < dataset->size(); i++)
        {
            dataset->at(i)->is_dirty = false;
            dataset->at(i)->with_distance = -1;
            dataset->at(i)->current_cluster = -1;
        }

        for (unsigned int i = 0; i < clusters->size(); i++)
        {
            clusters->at(i)->data->clear();
            clusters->at(i)->centroid->is_dirty = true;
            clusters->at(i)->centroid->current_cluster = i;
        }
    }

    double get_min_distance_between_clusters(
        vector<Cluster<double> *> *&clusters,
        Metrics<T> *&metrics)
    {
        double min_distance = 999999;
        double current_distance = 0;
        for (unsigned int i = 0; i < clusters->size(); i++)
        {
            for (unsigned int j = i + 1; j < clusters->size(); j++)
            {
                current_distance = metrics->get_distance(
                    clusters->at(i)->centroid->vector_data,
                    clusters->at(j)->centroid->vector_data);

                if (current_distance < min_distance)
                {
                    min_distance = current_distance;
                }
            }
        }
        return min_distance;
    }

    AssignmentType get_assignment_type()
    {
        return type_;
    }

    ClusterInsert(AssignmentType type,
                  vector<DataItem<T> *> *&dataset,
                  DataManager<T> *&data_manager,
                  Metrics<T> *&metrics)
    {
        type_ = type;
        switch (type_)
        {
        case LLOYDS_ASSIGNMENT:
            break;
        case LSH_ASSIGNMENT:
            lsh_execution_ = new LshExecution<T>();
            lsh_execution_->do_preprocessing(dataset, data_manager, metrics->get_metric_type());
            break;

        case CUBE_ASSIGNMENT:
            cube_execution_ = new CubeExecution<T>();
            cube_execution_->do_preprocessing(dataset, data_manager, metrics->get_metric_type());
            break;
        }
    }
    ClusterInsert() : type_(LLOYDS_ASSIGNMENT) {}

    ~ClusterInsert()
    {
        switch (type_)
        {
        case LSH_ASSIGNMENT:
            delete lsh_execution_;
            break;

        case CUBE_ASSIGNMENT:
            delete cube_execution_;
            break;

        default:
            break;
        }
    }
};

#endif