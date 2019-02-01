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

class ClusterUpdateKmeansTest : public testing::Test
{
  protected:
    ClusterUpdateKmeansTest()
    {
    }

    ~ClusterUpdateKmeansTest() override
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

        cluster_update = new ClusterUpdate<double>(KMEANS_UPDATE);
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

TEST_F(ClusterUpdateKmeansTest, SetNextMean)
{
    int index = 0;
    double sum = 0;
    double result = 0;
    for (unsigned int i = 0; i < mock_up_clusters->clusters->size(); i++)
    {
        if (mock_up_clusters->clusters->at(i)->data->size() > 2)
        {
            index = i;
            for (unsigned int j = 0; j < mock_up_clusters->clusters->at(i)->data->size(); j++)
            {
                sum += mock_up_clusters->clusters->at(i)->data->at(j)->vector_data->at(0);
            }
            result = sum / mock_up_clusters->clusters->at(i)->data->size();
            break;
        }
    }
    cluster_update->set_next_mean(mock_up_clusters->clusters->at(index));

    ASSERT_DOUBLE_EQ(result, mock_up_clusters->clusters->at(index)->centroid->vector_data->at(0));
}

TEST_F(ClusterUpdateKmeansTest, KmeansExecution)
{
    cluster_update->clustering_execute(mock_up_dataset->dataset, mock_up_clusters->clusters, data_manager, mock_up_dataset->eu_metrics, cluster_insert);

    for (unsigned int i = 0; i < mock_up_dataset->dataset->size(); i++)
    {
        ASSERT_TRUE(mock_up_dataset->dataset->at(i)->is_dirty);
        ASSERT_NE(mock_up_dataset->dataset->at(i)->current_cluster, -1);
        ASSERT_NE(mock_up_dataset->dataset->at(i)->with_distance, -1);
    }
}

} // namespace