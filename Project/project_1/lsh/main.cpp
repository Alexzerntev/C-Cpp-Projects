#include <iostream>
#include <string>
#include "./initialize/initialize_lsh.hpp"
#include "../common/data_manipulation/data_manager.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../common/data_manipulation/result_item.hpp"
#include "../brute_force/bf_execution.hpp"
#include "../common/metrics.hpp"
#include "./lsh_execution.hpp"
#include <ctime>
#include "sys/types.h"
#include "sys/sysinfo.h"

using namespace std;

int main(int argc, char *argv[])
{
    bool first_time = true;
    clock_t bf_begin, bf_end, lsh_begin, lsh_end, all_start, all_end;
    all_start = clock();
    // Firstly check if arguments from command line
    InitializeLsh *init = new InitializeLsh(argc, argv);
    while (1)
    {
        // If no arguments or arguments are invalid, execute manual initialization
        if (!init->is_valid())
        {
            init->manual_init(first_time);
        }

        // Objects that manages data and the actual data objects
        DataManager<int> *data_manager = new DataManager<int>();
        vector<DataItem<int> *> *input_data = data_manager->read_file(init->get_input_file());
        vector<DataItem<int> *> *query_data = data_manager->read_file(init->get_query_file());

        // If file reading gone well continue
        if (!data_manager->has_errors())
        {
            Metrics<int> *metric = new Metrics<int>(data_manager->get_metric_type());
            BfExecution<int> *bf_execution = new BfExecution<int>();
            LshExecution<int> *lsh_execution = new LshExecution<int>();
            lsh_execution->do_preprocessing(input_data, init, metric->get_metric_type());

            for (unsigned int i = 0; i < query_data->size(); i++)
            {
                bf_begin = clock();
                ResultItem *bf_result = bf_execution->find_nearest(
                    metric,
                    input_data,
                    query_data->at(i));
                bf_end = clock();
                double bf_elapsed = double(bf_end - bf_begin) / CLOCKS_PER_SEC;

                lsh_begin = clock();
                vector<ResultItem *> *lsh_result = lsh_execution->find_nearest(
                    metric,
                    query_data->at(i),
                    data_manager->get_radius(),
                    5);
                lsh_end = clock();
                double lsh_elapsed = double(lsh_end - lsh_begin) / CLOCKS_PER_SEC;

                data_manager->write_file(
                    query_data->at(i),
                    bf_result,
                    lsh_result,
                    metric->get_metric_type(),
                    init->get_output_file(),
                    "LSH",
                    lsh_elapsed,
                    bf_elapsed);

                // data_manager->write_file(
                //     query_data->at(i),
                //     lsh_result,
                //     metric->get_metric_type(),
                //     init->get_output_file(),
                //     "LSH",
                //     lsh_elapsed);

                delete bf_result;
                delete lsh_result;
            }

            delete metric;
            delete bf_execution;
        }

        data_manager->print_max_aprox_fraction();
        data_manager->print_mean_time();

        // House keeping
        DataItem<int> *temp;
        while (input_data->size() > 0)
        {
            temp = input_data->at(0);
            input_data->erase(input_data->begin());
            delete temp;
        }
        while (query_data->size() > 0)
        {
            temp = query_data->at(0);
            query_data->erase(query_data->begin());
            delete temp;
        }
        delete input_data;
        delete query_data;
        delete data_manager;

        all_end = clock();

        cout << "The total time is : " << double(all_end - all_start) / CLOCKS_PER_SEC << endl;
        if (!init->should_repeat())
        {
            break;
        }
    }
    delete init;
    exit(0);
}
