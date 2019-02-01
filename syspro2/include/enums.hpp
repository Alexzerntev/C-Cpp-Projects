#ifndef ENUMS_HPP
#define ENUMS_HPP

enum result
{
    error,
    success
};

enum orderType
{
    search_type,
    maxcount_type,
    mincount_type,
    workercount_type,
    path_distribution_type,
    exit_type,
    wrong_input
};
    // enum commandType
    // {
    //     search_type,
    //     all_document_frequancy_type,
    //     word_document_frequancy_type,
    //     term_frequancy_type,
    //     exit_type,
    //     wrong_input
    // };

#endif