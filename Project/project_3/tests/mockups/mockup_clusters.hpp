#ifndef MOCKUP_CLUSTERS_HPP
#define MOCKUP_CLUSTERS_HPP

#include <string>
#include <iostream>
#include <vector>
#include "../../common/data_manipulation/data_item.hpp"
#include "../../common/random_generator.hpp"
#include "../../common/metrics.hpp"
#include "../../common/enums.hpp"
#include "../../clustering/cluster_initialize.hpp"

using namespace std;

template <class T>
class MockUpClusters
{
  private:
    ClusterInitialize<T> *cluster_initialize;

  public:
    vector<Cluster<T> *> *clusters;

    MockUpClusters(vector<DataItem<T> *> *&dataset, Metrics<T> *&metrics)
    {
        cluster_initialize = new ClusterInitialize<T>();
        clusters = cluster_initialize->initialize_cluster(3, dataset, RANDOM_INITIALIZATION, metrics);
    }

    ~MockUpClusters()
    {
        Cluster<double> *temp_cluster;
        while (clusters->size() > 0)
        {
            temp_cluster = clusters->at(0);
            clusters->erase(clusters->begin());
            delete temp_cluster;
        }
        delete cluster_initialize;
    }
};

#endif
