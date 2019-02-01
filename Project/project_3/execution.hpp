#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include <iostream>
#include <ctime>
#include <map>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "./common/data_manipulation/data_manager.hpp"
#include "./common/argument_initialize.hpp"
#include "./common/metrics.hpp"
#include "./common/data_manipulation/user_data_item.hpp"
#include "./clustering/cluster_initialize.hpp"
#include "./clustering/cluster_insert.hpp"
#include "./clustering/cluster_update.hpp"
#include "./clustering/cluster.hpp"
#include "./evaluation/silhouette.hpp"
#include "./recomendation/sentiment.hpp"
#include "./recomendation/recomendation_helpers.hpp"

using namespace std;

class Execution
{

private:
protected:
public:

int virtual_users_lsh(int argc, char *argv[]);
int virtual_users_cluster(int argc, char *argv[]);
int users_lsh(int argc, char *argv[]);
int users_cluster(int argc, char *argv[]);




};

#endif