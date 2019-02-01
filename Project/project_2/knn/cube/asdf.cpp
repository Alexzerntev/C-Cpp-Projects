#include <iostream>
#include "./initialize/initialize_cube.hpp"
#include "../common/data_manipulation/data_manager.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../common/data_manipulation/result_item.hpp"
#include "../common/enums.hpp"
#include "../common/metrics.hpp"
#include "./cube_execution.hpp"
#include "../brute_force/bf_execution.hpp"

int main(int argc, char *argv[])
{
    bool first_time = true;
    clock_t bf_begin, bf_end, cube_begin, cube_end, all_start, all_end;
    all_start = clock();
    // Firstly check if arguments from command line
    InitializeCube *init = new InitializeCube(argc, argv);
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

        if (!data_manager->has_errors())
        {
            Metrics<int> *metric = new Metrics<int>(data_manager->get_metric_type());
            BfExecution<int> *bf_execution = new BfExecution<int>();
            CubeExecution<int> *cube_execution = new CubeExecution<int>();
            cube_execution->do_preprocessing(input_data, init->get_k(), metric->get_metric_type());

            for (unsigned int i = 0; i < query_data->size(); i++)
            {
                bf_begin = clock();
                ResultItem *bf_result = bf_execution->find_nearest(
                    metric,
                    input_data,
                    query_data->at(i));
                bf_end = clock();
                double bf_elapsed = double(bf_end - bf_begin) / CLOCKS_PER_SEC;

                cube_begin = clock();
                vector<ResultItem *> *cube_result = cube_execution->find_nearest(
                    metric,
                    query_data->at(i),
                    data_manager->get_radius(),
                    5,
                    init);
                cube_end = clock();
                double cube_elapsed = double(cube_end - cube_begin) / CLOCKS_PER_SEC;

                data_manager->write_file(
                    query_data->at(i),
                    bf_result,
                    cube_result,
                    metric->get_metric_type(),
                    init->get_output_file(),
                    "CUBE",
                    cube_elapsed,
                    bf_elapsed);

                delete bf_result;
                delete cube_result;
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

        if (!init->should_repeat())
        {
            break;
        }
    }
    delete init;
    exit(0);
}
