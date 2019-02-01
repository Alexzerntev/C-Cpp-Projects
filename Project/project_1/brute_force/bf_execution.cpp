#include "./bf_execution.hpp"

// template <class T>
// BfExecution<T>::BfExecution()
// {
// }

// template <class T>
// vector<ResultItem *> *BfExecution<T>::find_nearest(Metrics<T> *&metric, vector<DataItem<T> *> *&input_data, vector<DataItem<T> *> *&query_data, int R)
// {
//     unsigned int distance = 65534, current_distance = 0;
//     string current_id;
//     vector<ResultItem *> *result = new vector<ResultItem *>();

//     if (R == 0)
//     {
//         for (int i = 0; i < query_data->size(); i++)
//         {
//             for (int j = 0; j < input_data->size(); j++)
//             {
//                 current_distance = metric->get_distance(query_data[i]->vector_data, input_data[j]->vector_data);
//                 if (current_distance < distance)
//                 {
//                     distance = current_distance;
//                     current_id = input_data[j]->id;
//                 }
//             }
//         }
//         ResultItem *result_item = new ResultItem();
//         result_item->distance = distance;
//         result->push_back(result_item);
//     }
//     else
//     {
//         /* code */
//     }
//     return result;
// }