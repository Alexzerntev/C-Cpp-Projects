#include "gtest/gtest.h"
#include <unistd.h>
#include "./mockups/mockup_dataset.hpp"
#include "./mockups/mockup_clusters.hpp"
#include "../common/data_manipulation/data_manager.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../common/random_generator.hpp"
#include "../common/metrics.hpp"
#include "../common/enums.hpp"
#include "../clustering/cluster_initialize.hpp"
#include "../clustering/cluster_helpers.hpp"
#include "../clustering/cluster_insert.hpp"

namespace
{

class ClusterInsertLshTest : public testing::Test
{
  protected:
    ClusterInsertLshTest()
    {
    }

    ~ClusterInsertLshTest() override
    {
    }

    void SetUp() override
    {
        char cwd[200];
        getcwd(cwd, sizeof(cwd));
        string path(cwd);

        data_manager = new DataManager<double>();
        if (path.find("tests") != std::string::npos)
        {
            data_manager->read_config("mockups/cluster.conf");
        }
        else
        {
            data_manager->read_config("tests/mockups/cluster.conf");
        }

        mock_up_dataset = new MockUpDataset<double>();
        mock_up_clusters = new MockUpClusters<double>(mock_up_dataset->dataset, mock_up_dataset->eu_metrics);
        cluster_insert = new ClusterInsert<double>(LSH_ASSIGNMENT, mock_up_dataset->dataset, data_manager, mock_up_dataset->eu_metrics);
        cluster_insert->insert_to_cluster(mock_up_dataset->dataset, mock_up_clusters->clusters, data_manager, mock_up_dataset->eu_metrics);
    }

    void TearDown() override
    {
        delete mock_up_dataset;
        delete mock_up_clusters;
        delete cluster_insert;
        delete data_manager;
    }
    MockUpDataset<double> *mock_up_dataset;
    MockUpClusters<double> *mock_up_clusters;
    ClusterInsert<double> *cluster_insert;
    DataManager<double> *data_manager;
};

TEST_F(ClusterInsertLshTest, AllPointsInserted)
{
    cluster_insert->insert_to_cluster(mock_up_dataset->dataset, mock_up_clusters->clusters, mock_up_dataset->eu_metrics);

    for (unsigned int i = 0; i < mock_up_dataset->dataset->size(); i++)
    {
        if (is_center(mock_up_clusters->clusters, mock_up_dataset->dataset->at(i)->id))
        {
            continue;
        }
        ASSERT_TRUE(mock_up_dataset->dataset->at(i)->is_dirty);
        ASSERT_NE(mock_up_dataset->dataset->at(i)->current_cluster, -1);
        ASSERT_NE(mock_up_dataset->dataset->at(i)->with_distance, -1);
    }
}

} // namespace