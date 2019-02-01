#include "./validate_execution.hpp"

int ValidateExecution::users_lsh(int argc, char *argv[])
{
    clock_t start, finish;
    double elapsed;
    // Managers
    ArgumentInitialize *argument_initialize = NULL;
    DataManager<double> *data_manager = NULL;
    ClusterInitialize<double> *cluster_initialize = NULL;
    ClusterInsert<double> *cluster_insert = NULL;
    ClusterUpdate<double> *cluster_update = NULL;
    RandomGenerator *random_generator = NULL;

    // Silhouette<double> *silhouette = NULL;
    Metrics<double> *euclidean_metrics = NULL;
    Metrics<double> *cosine_metrics = NULL;
    Sentiment<double> *sentiment = NULL;
    LshExecution<double> *lsh_execution = NULL;

    // // Data
    vector<DataItem<double> *> *data = NULL;

    //>>>>>>>>>>
    vector<DataItem<double> *> *data2 = NULL;
    vector<DataItem<double> *> *result_data = NULL;
    DataItem<double> *result = NULL;
    vector<UserDataItem *> *user_data = NULL;
    vector<Cluster<double> *> *clusters = NULL;
    map<string, string> *coin_dictionary = NULL;
    map<string, double> *sentiment_dictionary = NULL;
    vector<string> *coins = NULL;

    // By value data
    vector<string> coin_result;
    try
    {
        start = clock(); // Timer

        argument_initialize = new ArgumentInitialize(argc, argv);
        data_manager = new DataManager<double>();
        data_manager->read_config(argument_initialize->get_config_file());
        cosine_metrics = new Metrics<double>(COSINE);
        euclidean_metrics = new Metrics<double>(EUCLIDEAN);
        coin_dictionary = data_manager->create_coin_dictionary("input_files/coins_queries.csv");
        sentiment_dictionary = data_manager->create_sentiment_dictionary("input_files/vader_lexicon.csv");
        user_data = data_manager->read_user_data(argument_initialize->get_input_file());
        coins = data_manager->get_distinct_coin_names("input_files/coins_queries.csv");
        data = data_manager->create_user_profiles(user_data, coins);
        //>>>>>>>>>>>>>>>>>>>>
        data2 = data_manager->create_user_profiles(user_data, coins);
        random_generator = new RandomGenerator();

        sentiment = new Sentiment<double>();
        sentiment->assign_sentiment(user_data, sentiment_dictionary);
        sentiment->fill_data_items(data, coins, user_data, coin_dictionary);
        normalize_data(data);

        //>>>>>>>>>>>>>>>>>>>>
        sentiment = new Sentiment<double>();
        sentiment->assign_sentiment(user_data, sentiment_dictionary);
        sentiment->fill_data_items(data2, coins, user_data, coin_dictionary);
        normalize_data(data2);

        //>>>>>>>>>>>>>>>>>>>>
        int random_result;
        vector<double> maes;
        unsigned int set_count = data2->size() / 10;

        //>>>> shuffle
        //>>>>>>>>>>>>>>>>>>>
        for (unsigned int i = 0; i < set_count; i++)
        {
            random_result = random_generator->get_uniform_distribution_random_with_size(data2->size());

            auto it = data->begin() + random_result;
            std::rotate(it, it + 1, data->end());

            auto it2 = data2->begin() + random_result;
            std::rotate(it2, it2 + 1, data2->end());
        }

        for (unsigned int i = 0; i < data2->size(); i++)
        {
            for (unsigned int j = 0; j < data2->at(i)->is_known->size(); j++)
            {
                if (data2->at(i)->is_known->at(j) == true)
                {
                    data2->at(i)->is_known->at(j) = false;
                    data2->at(i)->vector_data->at(j) = 0;
                }
            }
        }

        // ========================= LSH Cosine =======================================================================================
        cout << "Runing LSH Cosine..." << endl; //===================================
        lsh_execution = new LshExecution<double>();
        lsh_execution->do_preprocessing(data, data_manager, COSINE);

        //>>>>>>>>>>
        double local_mae = 0;
        int cnt = 0;
        bool flag = false;
        //>>>>>>>>>>>>>>>>>>>>>>>>>>>
        for (int j = 0; j < 10; j++)
        {
            for (unsigned int i = 0; i < data->size(); i++)
            {
                data->at(i)->is_in_use = true;
            }

            for (unsigned int i = 0; i < set_count; i++)
            {
                data->at((j * set_count) + i)->is_in_use = false;
            }

            local_mae = 0;

            cnt = 0;
            for (unsigned int i = j * set_count; i < set_count * (j + 1); i++)
            {
                result_data = lsh_execution->find_nearest_lsh(cosine_metrics, data2->at(i), 9999, 20);

                result = execute_recomendation(data2->at(i), result_data, false);

                for (unsigned int k = 0; k < data->at(i)->is_known->size(); k++)
                {
                    if (data->at(i)->is_known->at(k) == true && data->at(i)->vector_data->at(k) != 0)
                    {
                        flag = true;
                        // cout << data->at(i)->vector_data->at(k) << "is know: " << data->at(i)->is_known->at(k) << " " << data2->at(i)->vector_data->at(k) << "is know: " << data2->at(i)->is_known->at(k) << endl;
                        local_mae = local_mae + abs(result->vector_data->at(k) - data->at(i)->vector_data->at(k));
                        break;
                    }
                }

                if (flag)
                {
                    flag = false;
                    cnt++;
                }

                delete result;
                delete result_data;
            }

            local_mae = local_mae / cnt;
            maes.push_back(local_mae);
        }

        finish = clock(); // Timer Finish
        elapsed = double(finish - start) / CLOCKS_PER_SEC;
        data_manager->write_file_time(elapsed, argument_initialize->get_output_file());

        //>>>>>>>>>>>>>>>>>>>>>>>>>>
        double mae = 0;
        for (unsigned int i = 0; i < maes.size(); i++)
        {
            mae = mae + maes.at(i);
        }
        mae = mae / maes.size();
        data_manager->write_file_evaluation(mae, argument_initialize->get_output_file(), true);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // House keeping

    DataItem<double> *temp_data_item;
    while (data->size() > 0)
    {
        temp_data_item = data->at(0);
        data->erase(data->begin());
        delete temp_data_item;
    }

    // >>>>>>>>>>>>>>>>>>>
    DataItem<double> *temp_data_item2;
    while (data2->size() > 0)
    {
        temp_data_item2 = data2->at(0);
        data2->erase(data2->begin());
        delete temp_data_item2;
    }

    UserDataItem *temp_user_data_item;
    while (user_data->size() > 0)
    {
        temp_user_data_item = user_data->at(0);
        user_data->erase(user_data->begin());
        delete temp_user_data_item;
    }

    delete cluster_initialize;
    delete cluster_update;
    delete cluster_insert;
    delete clusters;
    delete cosine_metrics;
    delete euclidean_metrics;
    delete data;
    delete user_data;
    delete data_manager;
    delete coin_dictionary;
    delete sentiment_dictionary;
    delete argument_initialize;
    delete sentiment;
    delete coins;
    delete lsh_execution;
    return 0;
}

int ValidateExecution::users_cluster(int argc, char *argv[])
{
    clock_t start, finish;
    double elapsed;
    // Managers
    ArgumentInitialize *argument_initialize = NULL;
    DataManager<double> *data_manager = NULL;
    ClusterInitialize<double> *cluster_initialize = NULL;
    ClusterInsert<double> *cluster_insert = NULL;
    ClusterUpdate<double> *cluster_update = NULL;

    //>>>>>>>>>>>>>>
    RandomGenerator *random_generator = NULL;

    // Silhouette<double> *silhouette = NULL;
    Metrics<double> *euclidean_metrics = NULL;
    Metrics<double> *cosine_metrics = NULL;
    Sentiment<double> *sentiment = NULL;
    LshExecution<double> *lsh_execution = NULL;

    // // Data
    vector<DataItem<double> *> *data = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    vector<DataItem<double> *> *data2 = NULL;
    vector<DataItem<double> *> *result_data = NULL;
    DataItem<double> *result = NULL;
    vector<UserDataItem *> *user_data = NULL;
    vector<Cluster<double> *> *clusters = NULL;
    map<string, string> *coin_dictionary = NULL;
    map<string, double> *sentiment_dictionary = NULL;
    vector<string> *coins = NULL;

    // By value data
    vector<string> coin_result;
    try
    {
        start = clock(); // Timer

        InitializationType init_type = RANDOM_INITIALIZATION;
        AssignmentType assign_type = LSH_ASSIGNMENT;
        UpdateType update_type = KMEANS_UPDATE;

        argument_initialize = new ArgumentInitialize(argc, argv);
        data_manager = new DataManager<double>();
        data_manager->read_config(argument_initialize->get_config_file());
        cosine_metrics = new Metrics<double>(COSINE);
        euclidean_metrics = new Metrics<double>(EUCLIDEAN);
        coin_dictionary = data_manager->create_coin_dictionary("input_files/coins_queries.csv");
        sentiment_dictionary = data_manager->create_sentiment_dictionary("input_files/vader_lexicon.csv");
        user_data = data_manager->read_user_data(argument_initialize->get_input_file());
        coins = data_manager->get_distinct_coin_names("input_files/coins_queries.csv");
        data = data_manager->create_user_profiles(user_data, coins);

        //>>>>>>>>>>>>>>>>>>>>
        data2 = data_manager->create_user_profiles(user_data, coins);
        random_generator = new RandomGenerator();

        sentiment = new Sentiment<double>();
        sentiment->assign_sentiment(user_data, sentiment_dictionary);
        sentiment->fill_data_items(data, coins, user_data, coin_dictionary);
        normalize_data(data);

        //>>>>>>>>>>>>>>>>>>>>
        sentiment = new Sentiment<double>();
        sentiment->assign_sentiment(user_data, sentiment_dictionary);
        sentiment->fill_data_items(data2, coins, user_data, coin_dictionary);
        normalize_data(data2);

        //>>>>>>>>>>>>>>>>>>>>
        int random_result;
        vector<double> maes;
        unsigned int set_count = data2->size() / 10;

        //>>>> shuffle
        //>>>>>>>>>>>>>>>>>>>
        for (unsigned int i = 0; i < set_count; i++)
        {
            random_result = random_generator->get_uniform_distribution_random_with_size(data2->size());

            auto it = data->begin() + random_result;
            std::rotate(it, it + 1, data->end());

            auto it2 = data2->begin() + random_result;
            std::rotate(it2, it2 + 1, data2->end());
        }

        for (unsigned int i = 0; i < data2->size(); i++)
        {
            for (unsigned int j = 0; j < data2->at(i)->is_known->size(); j++)
            {
                if (data2->at(i)->is_known->at(j) == true)
                {
                    data2->at(i)->is_known->at(j) = false;
                    data2->at(i)->vector_data->at(j) = 0;
                }
            }
        }

        // ========================= Clustering ========================================================================================

        cout << "Runing Clustering..." << endl; //===================================

        cluster_initialize = new ClusterInitialize<double>();
        clusters = cluster_initialize->initialize_cluster(data_manager->get_number_of_clusters(), data, init_type, euclidean_metrics);

        cluster_insert = new ClusterInsert<double>(assign_type, data, data_manager, euclidean_metrics);
        cluster_insert->insert_to_cluster(data, clusters, data_manager, euclidean_metrics);

        cluster_update = new ClusterUpdate<double>(update_type);
        cluster_update->clustering_execute(data, clusters, data_manager, euclidean_metrics, cluster_insert);

        //>>>>>>>>>>
        double local_mae = 0;
        int cnt = 0;
        bool flag = false;
        //>>>>>>>>>>>>>>>>>>>>>>>>>>>
        for (int j = 0; j < 10; j++)
        {
            for (unsigned int i = 0; i < data->size(); i++)
            {
                data->at(i)->is_in_use = true;
            }

            for (unsigned int i = 0; i < set_count; i++)
            {
                data->at((j * set_count) + i)->is_in_use = false;
            }

            local_mae = 0;

            cnt = 0;
            for (unsigned int i = j * set_count; i < set_count * (j + 1); i++)
            {
                result = execute_recomendation(data2->at(i), clusters->at(data->at(i)->current_cluster)->data, true);

                for (unsigned int k = 0; k < data->at(i)->is_known->size(); k++)
                {
                    if (data->at(i)->is_known->at(k) == true && data->at(i)->vector_data->at(k) != 0)
                    {
                        flag = true;
                        // cout << data->at(i)->vector_data->at(k) << "is know: " << data->at(i)->is_known->at(k) << " " << data2->at(i)->vector_data->at(k) << "is know: " << data2->at(i)->is_known->at(k) << endl;
                        local_mae = local_mae + abs(result->vector_data->at(k) - data2->at(i)->vector_data->at(k));
                        break;
                    }
                }

                if (flag)
                {
                    flag = false;
                    cnt++;
                }

                delete result;
                delete result_data;
            }

            local_mae = local_mae / cnt;
            maes.push_back(local_mae);
        }

        finish = clock(); // Timer Finish
        elapsed = double(finish - start) / CLOCKS_PER_SEC;
        data_manager->write_file_time(elapsed, argument_initialize->get_output_file());

        //>>>>>>>>>>>>>>>>>>>>>>>>>>
        double mae = 0;
        for (unsigned int i = 0; i < maes.size(); i++)
        {
            mae = mae + maes.at(i);
        }
        mae = mae / maes.size();
        data_manager->write_file_evaluation(mae, argument_initialize->get_output_file(), false);

        Cluster<double> *temp_cluster;
        while (clusters->size() > 0)
        {
            temp_cluster = clusters->at(0);
            clusters->erase(clusters->begin());
            delete temp_cluster;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // House keeping

    DataItem<double> *temp_data_item;
    while (data->size() > 0)
    {
        temp_data_item = data->at(0);
        data->erase(data->begin());
        delete temp_data_item;
    }

    // >>>>>>>>>>>>>>>>>>>
    DataItem<double> *temp_data_item2;
    while (data2->size() > 0)
    {
        temp_data_item2 = data2->at(0);
        data2->erase(data2->begin());
        delete temp_data_item2;
    }

    UserDataItem *temp_user_data_item;
    while (user_data->size() > 0)
    {
        temp_user_data_item = user_data->at(0);
        user_data->erase(user_data->begin());
        delete temp_user_data_item;
    }

    delete cluster_initialize;
    delete cluster_update;
    delete cluster_insert;
    delete clusters;
    delete cosine_metrics;
    delete euclidean_metrics;
    delete data;
    delete user_data;
    delete data_manager;
    delete coin_dictionary;
    delete sentiment_dictionary;
    delete argument_initialize;
    delete sentiment;
    delete coins;
    delete lsh_execution;
    return 0;
}

int ValidateExecution::virtual_users_lsh(int argc, char *argv[])
{
    // Time
    clock_t start, finish;
    double elapsed;
    // Managers
    ArgumentInitialize *argument_initialize = NULL;
    DataManager<double> *data_manager = NULL;
    ClusterInitialize<double> *cluster_initialize = NULL;
    ClusterInsert<double> *cluster_insert = NULL;
    ClusterUpdate<double> *cluster_update = NULL;

    ClusterInitialize<double> *cluster_initialize2 = NULL;
    ClusterInsert<double> *cluster_insert2 = NULL;
    ClusterUpdate<double> *cluster_update2 = NULL;
    vector<Cluster<double> *> *clusters2 = NULL;

    // Silhouette<double> *silhouette = NULL;
    Metrics<double> *euclidean_metrics = NULL;
    Metrics<double> *cosine_metrics = NULL;
    Sentiment<double> *sentiment = NULL;
    LshExecution<double> *lsh_execution = NULL;

    // // Data
    vector<DataItem<double> *> *data = NULL;
    vector<DataItem<double> *> *new_data = NULL;
    vector<DataItem<double> *> *result_data = NULL;
    vector<DataItem<double> *> *old_data = NULL;
    DataItem<double> *result = NULL;
    vector<UserDataItem *> *user_data = NULL;
    vector<UserDataItem *> *new_user_data = NULL;
    vector<Cluster<double> *> *clusters = NULL;
    map<string, string> *coin_dictionary = NULL;
    map<string, double> *sentiment_dictionary = NULL;
    vector<string> *coins = NULL;

    // By value data
    vector<string> coin_result;
    try
    {
        start = clock(); // Timer

        InitializationType init_type = RANDOM_INITIALIZATION;
        AssignmentType assign_type = LSH_ASSIGNMENT;
        UpdateType update_type = KMEANS_UPDATE;

        argument_initialize = new ArgumentInitialize(argc, argv);
        data_manager = new DataManager<double>();
        data_manager->read_config(argument_initialize->get_config_file());
        cosine_metrics = new Metrics<double>(COSINE);
        euclidean_metrics = new Metrics<double>(EUCLIDEAN);
        coin_dictionary = data_manager->create_coin_dictionary("input_files/coins_queries.csv");
        sentiment_dictionary = data_manager->create_sentiment_dictionary("input_files/vader_lexicon.csv");
        user_data = data_manager->read_user_data(argument_initialize->get_input_file());
        coins = data_manager->get_distinct_coin_names("input_files/coins_queries.csv");

        old_data = data_manager->read_data("input_files/input2.dat");

        cluster_initialize2 = new ClusterInitialize<double>();
        clusters2 = cluster_initialize2->initialize_cluster(100, old_data, init_type, euclidean_metrics);

        cluster_insert2 = new ClusterInsert<double>(assign_type, old_data, data_manager, euclidean_metrics);
        cluster_insert2->insert_to_cluster(old_data, clusters2, data_manager, euclidean_metrics);

        cluster_update2 = new ClusterUpdate<double>(update_type);
        cluster_update2->clustering_execute(old_data, clusters2, data_manager, euclidean_metrics, cluster_insert2);

        new_user_data = generate_dataset_from_cluster(clusters2, user_data);

        new_data = data_manager->create_user_profiles(new_user_data, coins);
        sentiment = new Sentiment<double>();
        sentiment->assign_sentiment(new_user_data, sentiment_dictionary);
        sentiment->fill_data_items(new_data, coins, new_user_data, coin_dictionary);
        normalize_data(new_data);

        data = data_manager->create_user_profiles(user_data, coins);
        sentiment = new Sentiment<double>();
        sentiment->assign_sentiment(user_data, sentiment_dictionary);
        sentiment->fill_data_items(data, coins, user_data, coin_dictionary);
        normalize_data(data);

        cout << "Runing LSH Cosine With Virtual Users..." << endl;
        lsh_execution = new LshExecution<double>();
        lsh_execution->do_preprocessing(new_data, data_manager, COSINE);

        data_manager->write_algorithm("Cosine LSH", argument_initialize->get_output_file());
        double value = 0;
        double sum = 0;
        int count = 0;
        int idx = 0;
        bool flag = false;
        for (unsigned int i = 0; i < data->size(); i++)
        {
            for (unsigned int j = 0; j < data->at(i)->is_known->size(); j++)
            {
                if (data->at(i)->is_known->at(j) == true && data->at(i)->vector_data->at(j) != 0)
                {
                    idx = j;
                    count++;
                    flag = true;
                    data->at(i)->is_known->at(j) = false;
                    value = data->at(i)->vector_data->at(j);
                    data->at(i)->vector_data->at(j) = 0;
                }
            }

            result_data = lsh_execution->find_nearest_lsh(cosine_metrics, data->at(i), 9999, 20);
            result = execute_recomendation(data->at(i), result_data, false);

            if (flag)
            {
                sum = sum + abs(result->vector_data->at(idx) - value);
                flag = false;
            }

            delete result;
            delete result_data;
        }
        finish = clock(); // Timer Finish
        elapsed = double(finish - start) / CLOCKS_PER_SEC;
        data_manager->write_file_time(elapsed, argument_initialize->get_output_file());

        sum = sum / count;
        data_manager->write_file_evaluation(sum, argument_initialize->get_output_file(), true);

        DataItem<double> *temp_data_item2;
        while (new_data->size() > 0)
        {
            temp_data_item2 = new_data->at(0);
            new_data->erase(new_data->begin());
            delete temp_data_item2;
        }
        UserDataItem *temp_user_data_item2;
        while (new_user_data->size() > 0)
        {
            temp_user_data_item2 = new_user_data->at(0);
            new_user_data->erase(new_user_data->begin());
            delete temp_user_data_item2;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // House keeping

    DataItem<double> *temp_data_item;
    while (data->size() > 0)
    {
        temp_data_item = data->at(0);
        data->erase(data->begin());
        delete temp_data_item;
    }

    UserDataItem *temp_user_data_item;
    while (user_data->size() > 0)
    {
        temp_user_data_item = user_data->at(0);
        user_data->erase(user_data->begin());
        delete temp_user_data_item;
    }

    delete cluster_initialize;
    delete cluster_update;
    delete cluster_insert;
    delete clusters;
    delete cosine_metrics;
    delete euclidean_metrics;
    delete data;
    delete user_data;
    delete data_manager;
    delete coin_dictionary;
    delete sentiment_dictionary;
    delete argument_initialize;
    delete sentiment;
    delete coins;
    delete lsh_execution;
    return 0;
}

int ValidateExecution::virtual_users_cluster(int argc, char *argv[])
{
    // Time
    clock_t start, finish;
    double elapsed;
    // Managers
    ArgumentInitialize *argument_initialize = NULL;
    DataManager<double> *data_manager = NULL;
    ClusterInitialize<double> *cluster_initialize = NULL;
    ClusterInsert<double> *cluster_insert = NULL;
    ClusterUpdate<double> *cluster_update = NULL;

    ClusterInitialize<double> *cluster_initialize2 = NULL;
    ClusterInsert<double> *cluster_insert2 = NULL;
    ClusterUpdate<double> *cluster_update2 = NULL;
    vector<Cluster<double> *> *clusters2 = NULL;

    // Silhouette<double> *silhouette = NULL;
    Metrics<double> *euclidean_metrics = NULL;
    Metrics<double> *cosine_metrics = NULL;
    Sentiment<double> *sentiment = NULL;
    LshExecution<double> *lsh_execution = NULL;

    // // Data
    vector<DataItem<double> *> *data = NULL;
    vector<DataItem<double> *> *new_data = NULL;
    vector<DataItem<double> *> *result_data = NULL;
    vector<DataItem<double> *> *old_data = NULL;
    DataItem<double> *result = NULL;
    vector<UserDataItem *> *user_data = NULL;
    vector<UserDataItem *> *new_user_data = NULL;
    vector<Cluster<double> *> *clusters = NULL;
    map<string, string> *coin_dictionary = NULL;
    map<string, double> *sentiment_dictionary = NULL;
    vector<string> *coins = NULL;

    // By value data
    vector<string> coin_result;
    try
    {
        start = clock(); // Timer

        InitializationType init_type = RANDOM_INITIALIZATION;
        AssignmentType assign_type = LSH_ASSIGNMENT;
        UpdateType update_type = KMEANS_UPDATE;

        argument_initialize = new ArgumentInitialize(argc, argv);
        data_manager = new DataManager<double>();
        data_manager->read_config(argument_initialize->get_config_file());
        cosine_metrics = new Metrics<double>(COSINE);
        euclidean_metrics = new Metrics<double>(EUCLIDEAN);
        coin_dictionary = data_manager->create_coin_dictionary("input_files/coins_queries.csv");
        sentiment_dictionary = data_manager->create_sentiment_dictionary("input_files/vader_lexicon.csv");
        user_data = data_manager->read_user_data(argument_initialize->get_input_file());
        coins = data_manager->get_distinct_coin_names("input_files/coins_queries.csv");

        old_data = data_manager->read_data("input_files/input2.dat");

        cluster_initialize2 = new ClusterInitialize<double>();
        clusters2 = cluster_initialize2->initialize_cluster(10, old_data, init_type, euclidean_metrics);

        cluster_insert2 = new ClusterInsert<double>(assign_type, old_data, data_manager, euclidean_metrics);
        cluster_insert2->insert_to_cluster(old_data, clusters2, data_manager, euclidean_metrics);

        cluster_update2 = new ClusterUpdate<double>(update_type);
        cluster_update2->clustering_execute(old_data, clusters2, data_manager, euclidean_metrics, cluster_insert2);

        new_user_data = generate_dataset_from_cluster(clusters2, user_data);

        new_data = data_manager->create_user_profiles(new_user_data, coins);
        sentiment = new Sentiment<double>();
        sentiment->assign_sentiment(new_user_data, sentiment_dictionary);
        sentiment->fill_data_items(new_data, coins, new_user_data, coin_dictionary);
        normalize_data(new_data);

        data = data_manager->create_user_profiles(user_data, coins);
        sentiment = new Sentiment<double>();
        sentiment->assign_sentiment(user_data, sentiment_dictionary);
        sentiment->fill_data_items(data, coins, user_data, coin_dictionary);
        normalize_data(data);

        cout << "Runing Clustering With Virtual Users..." << endl;

        cluster_initialize = new ClusterInitialize<double>();
        clusters = cluster_initialize->initialize_cluster(5, new_data, init_type, euclidean_metrics);

        cluster_insert = new ClusterInsert<double>(assign_type, new_data, data_manager, euclidean_metrics);
        cluster_insert->insert_to_cluster(new_data, clusters, data_manager, euclidean_metrics);

        cluster_update = new ClusterUpdate<double>(update_type);
        cluster_update->clustering_execute(new_data, clusters, data_manager, euclidean_metrics, cluster_insert);

        int idx = -1;
        double value = 999999;
        double dist = 0;
        double val = 0;
        double sum = 0;
        bool flag = false;
        int count = 0;
        int index = 0;
        data_manager->write_algorithm("Clustering", argument_initialize->get_output_file());
        for (unsigned int i = 0; i < data->size(); i++)
        {
            for (unsigned int j = 0; j < data->at(i)->is_known->size(); j++)
            {
                if (data->at(i)->is_known->at(j) == true && data->at(i)->vector_data->at(j) != 0)
                {
                    index = j;
                    count++;
                    flag = true;
                    data->at(i)->is_known->at(j) = false;
                    val = data->at(i)->vector_data->at(j);
                    data->at(i)->vector_data->at(j) = 0;
                }
            }

            for (unsigned int j = 0; j < clusters->size(); j++)
            {
                dist = euclidean_metrics->get_distance(clusters->at(j)->centroid->vector_data, data->at(i)->vector_data);
                if (dist < value)
                {
                    value = dist;
                    idx = j;
                }
            }
            result = execute_recomendation(data->at(i), clusters->at(idx)->data, true);

            if (flag)
            {
                sum = sum + abs(result->vector_data->at(index) - val);
                flag = false;
            }

            idx = -1;
            value = 999999;
            delete result;
            delete result_data;
        }
        finish = clock(); // Timer Finish
        elapsed = double(finish - start) / CLOCKS_PER_SEC;
        data_manager->write_file_time(elapsed, argument_initialize->get_output_file());

        sum = sum / count;
        data_manager->write_file_evaluation(sum, argument_initialize->get_output_file(), false);

        Cluster<double> *temp_cluster;
        while (clusters->size() > 0)
        {
            temp_cluster = clusters->at(0);
            clusters->erase(clusters->begin());
            delete temp_cluster;
        }
        DataItem<double> *temp_data_item2;
        while (new_data->size() > 0)
        {
            temp_data_item2 = new_data->at(0);
            new_data->erase(new_data->begin());
            delete temp_data_item2;
        }

        UserDataItem *temp_user_data_item2;
        while (new_user_data->size() > 0)
        {
            temp_user_data_item2 = new_user_data->at(0);
            new_user_data->erase(new_user_data->begin());
            delete temp_user_data_item2;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // House keeping

    DataItem<double> *temp_data_item;
    while (data->size() > 0)
    {
        temp_data_item = data->at(0);
        data->erase(data->begin());
        delete temp_data_item;
    }

    UserDataItem *temp_user_data_item;
    while (user_data->size() > 0)
    {
        temp_user_data_item = user_data->at(0);
        user_data->erase(user_data->begin());
        delete temp_user_data_item;
    }

    delete cluster_initialize;
    delete cluster_update;
    delete cluster_insert;
    delete clusters;
    delete cosine_metrics;
    delete euclidean_metrics;
    delete data;
    delete user_data;
    delete data_manager;
    delete coin_dictionary;
    delete sentiment_dictionary;
    delete argument_initialize;
    delete sentiment;
    delete coins;
    delete lsh_execution;
    return 0;
}