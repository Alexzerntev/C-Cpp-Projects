#include <iostream>
#include <ctime>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "./common/data_manipulation/data_manager.hpp"
#include "./common/argument_initialize.hpp"
#include "./common/metrics.hpp"
#include "./clustering/cluster_initialize.hpp"
#include "./clustering/cluster_insert.hpp"
#include "./clustering/cluster_update.hpp"
#include "./clustering/cluster.hpp"
#include "./evaluation/silhouette.hpp"

int main(int argc, char *argv[])
{
    clock_t start, finish;
    start = clock();
    // Managers
    ArgumentInitialize *argument_initialize = NULL;
    DataManager<double> *data_manager = NULL;
    ClusterInitialize<double> *cluster_initialize = NULL;
    ClusterInsert<double> *cluster_insert = NULL;
    ClusterUpdate<double> *cluster_update = NULL;
    Silhouette<double> *silhouette = NULL;
    Metrics<double> *metrics = NULL;

    // Data
    vector<DataItem<double> *> *dataset = NULL;
    vector<Cluster<double> *> *clusters = NULL;

    // ========== Here are the parameters for algorithm execution =============
    // ---------- Initialization type ----------
    InitializationType init_type = RANDOM_INITIALIZATION;
    // InitializationType init_type = KMEANS_PP_INITIALIZATION;

    // ---------- Assignment Type ----------
    //AssignmentType assign_type = LLOYDS_ASSIGNMENT;
    AssignmentType assign_type = LSH_ASSIGNMENT;
    // AssignmentType assign_type = CUBE_ASSIGNMENT;

    // ---------- Update Type -----------
    UpdateType update_type = KMEANS_UPDATE;
    // UpdateType update_type = PAM_UPDATE;

    try
    {
        data_manager = new DataManager<double>();
        argument_initialize = new ArgumentInitialize(argc, argv);
        data_manager->read_config(argument_initialize->get_config_file());
        dataset = data_manager->read_data(argument_initialize->get_input_file());

        metrics = new Metrics<double>(argument_initialize->get_metric());

        cluster_initialize = new ClusterInitialize<double>();
        clusters = cluster_initialize->initialize_cluster(data_manager->get_number_of_clusters(), dataset, init_type, metrics);

        cluster_insert = new ClusterInsert<double>(assign_type, dataset, data_manager, metrics);
        cluster_insert->insert_to_cluster(dataset, clusters, data_manager, metrics);

        cluster_update = new ClusterUpdate<double>(update_type);
        cluster_update->clustering_execute(dataset, clusters, data_manager, metrics, cluster_insert);

        finish = clock();
        cout << double(finish - start) / CLOCKS_PER_SEC << endl;
        silhouette->evaluate(clusters, dataset, metrics);

        data_manager->write_file(
            argument_initialize->get_output_file(),
            init_type,
            assign_type,
            update_type,
            metrics,
            clusters,
            dataset,
            double(finish - start) / CLOCKS_PER_SEC,
            argument_initialize->get_is_complete());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // House keeping

    if (update_type == KMEANS_UPDATE)
    {
        for (unsigned int i = 0; i < clusters->size(); i++)
        {
            delete clusters->at(i)->centroid;
        }
    }

    DataItem<double> *temp_data_item;
    while (dataset->size() > 0)
    {
        temp_data_item = dataset->at(0);
        dataset->erase(dataset->begin());
        delete temp_data_item;
    }

    Cluster<double> *temp_cluster;
    while (clusters->size() > 0)
    {
        temp_cluster = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp_cluster;
    }

    delete cluster_initialize;
    delete cluster_update;
    delete cluster_insert;
    delete clusters;
    delete metrics;
    delete dataset;
    delete data_manager;
    delete silhouette;
    delete argument_initialize;
    return 0;
}