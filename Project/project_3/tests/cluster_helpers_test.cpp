#include "gtest/gtest.h"
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

class ClusterHelpersTest : public testing::Test
{
  protected:
    ClusterHelpersTest()
    {
    }

    ~ClusterHelpersTest() override
    {
    }

    void SetUp() override
    {
        mock_up_dataset = new MockUpDataset<double>();
        mock_up_clusters = new MockUpClusters<double>(mock_up_dataset->dataset, mock_up_dataset->eu_metrics);
        cluster_insert = new ClusterInsert<double>();
        cluster_insert->insert_to_cluster(mock_up_dataset->dataset, mock_up_clusters->clusters, mock_up_dataset->eu_metrics);
    }

    void TearDown() override
    {
        delete mock_up_dataset;
        delete mock_up_clusters;
        delete cluster_insert;
    }
    MockUpDataset<double> *mock_up_dataset;
    MockUpClusters<double> *mock_up_clusters;
    ClusterInsert<double> *cluster_insert;
};

TEST_F(ClusterHelpersTest, DeleteById)
{
    unsigned long int id = 0;
    int index = 0;
    int current_size = 0;

    for (unsigned int i = 0; i < mock_up_clusters->clusters->size(); i++)
    {
        if (mock_up_clusters->clusters->at(i)->data->size() != 0)
        {
            index = i;
            id = mock_up_clusters->clusters->at(i)->data->at(0)->id;
            current_size = mock_up_clusters->clusters->at(i)->data->size();
            break;
        }
    }
    delete_by_id(mock_up_clusters->clusters->at(index), id);

    for (unsigned int i = 0; i < mock_up_clusters->clusters->at(index)->data->size(); i++)
    {
        if (mock_up_clusters->clusters->at(index)->data->at(i)->id == id)
        {
            FAIL();
        }
    }
    ASSERT_EQ(current_size, mock_up_clusters->clusters->at(index)->data->size() + 1);
}

TEST_F(ClusterHelpersTest, IsCenter)
{
    cluster_insert->insert_to_cluster(mock_up_dataset->dataset, mock_up_clusters->clusters, mock_up_dataset->eu_metrics);
    int id = mock_up_clusters->clusters->at(0)->centroid->id;
    ASSERT_TRUE(is_center(mock_up_clusters->clusters, id));

    for (unsigned int i = 0; i < mock_up_clusters->clusters->size(); i++)
    {
        if (mock_up_clusters->clusters->at(i)->data->size() != 0)
        {
            id = mock_up_clusters->clusters->at(i)->data->at(0)->id;
        }
    }

    ASSERT_FALSE(is_center(mock_up_clusters->clusters, id));
}

} // namespace