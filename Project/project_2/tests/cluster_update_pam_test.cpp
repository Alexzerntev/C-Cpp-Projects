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
#include "../clustering/cluster_update.hpp"

namespace
{

class ClusterUpdatePamTest : public testing::Test
{
  protected:
    ClusterUpdatePamTest()
    {
    }

    ~ClusterUpdatePamTest() override
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

        cluster_insert = new ClusterInsert<double>(LLOYDS_ASSIGNMENT, mock_up_dataset->dataset, data_manager, mock_up_dataset->eu_metrics);
        cluster_insert->insert_to_cluster(mock_up_dataset->dataset, mock_up_clusters->clusters, data_manager, mock_up_dataset->eu_metrics);

        cluster_update = new ClusterUpdate<double>(PAM_UPDATE);
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
    ClusterUpdate<double> *cluster_update;
    DataManager<double> *data_manager;
};

TEST_F(ClusterUpdatePamTest, GetObjectiveCost)
{
    mock_up_clusters->clusters->at(0)->data->clear();
    ASSERT_DOUBLE_EQ(cluster_update->get_objective_cost(mock_up_clusters->clusters, 0, mock_up_dataset->eu_metrics), 0);

    for (unsigned int i = 1; i < mock_up_clusters->clusters->size(); i++)
    {
        if (mock_up_clusters->clusters->at(i)->data->size() > 0)
        {
            ASSERT_NE(cluster_update->get_objective_cost(mock_up_clusters->clusters, i, mock_up_dataset->eu_metrics), 0);
        }
    }
}

TEST_F(ClusterUpdatePamTest, PamExecution)
{
    cluster_update->clustering_execute(mock_up_dataset->dataset, mock_up_clusters->clusters, data_manager, mock_up_dataset->eu_metrics, cluster_insert);

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