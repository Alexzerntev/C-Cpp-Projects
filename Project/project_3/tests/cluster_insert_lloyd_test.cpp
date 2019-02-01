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

class ClusterInsertLloydTest : public testing::Test
{
  protected:
    ClusterInsertLloydTest()
    {
    }

    ~ClusterInsertLloydTest() override
    {
    }

    void SetUp() override
    {
        char cwd[200];
        getcwd(cwd, sizeof(cwd));
        string path(cwd);

        mock_up_dataset = new MockUpDataset<double>();
        mock_up_clusters = new MockUpClusters<double>(mock_up_dataset->dataset, mock_up_dataset->eu_metrics);
        cluster_insert = new ClusterInsert<double>();
        cluster_insert->insert_to_cluster(mock_up_dataset->dataset, mock_up_clusters->clusters, mock_up_dataset->eu_metrics);

        data_manager = new DataManager<double>();
        if (path.find("tests") != std::string::npos)
        {
            data_manager->read_config("mockups/cluster.conf");
        }
        else
        {
            data_manager->read_config("tests/mockups/cluster.conf");
        }
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

TEST_F(ClusterInsertLloydTest, MinDistanceBetweenClusters)
{
    vector<Cluster<double> *> *clusters = new vector<Cluster<double> *>();

    for (unsigned int i = 0; i < 3; i++)
    {
        DataItem<double> *data_item = new DataItem<double>();
        data_item->id = i;
        data_item->vector_data = new vector<double>();
        data_item->vector_data->push_back(i == 2 ? 10 : i);
        data_item->vector_data->push_back(i == 2 ? 10 : i);
        clusters->push_back(new Cluster<double>(i, data_item));
    }

    ASSERT_DOUBLE_EQ(cluster_insert->get_min_distance_between_clusters(clusters, mock_up_dataset->eu_metrics), sqrt(2));

    delete clusters->at(0)->centroid;
    delete clusters->at(1)->centroid;
    delete clusters->at(2)->centroid;
    Cluster<double> *temp1 = clusters->at(0);
    Cluster<double> *temp2 = clusters->at(1);
    Cluster<double> *temp3 = clusters->at(2);
    clusters->clear();
    delete temp1;
    delete temp2;
    delete temp3;
    delete clusters;
}

TEST_F(ClusterInsertLloydTest, ResetDataset)
{
    cluster_insert->reset_dataset(mock_up_clusters->clusters, mock_up_dataset->dataset);

    for (unsigned int i = 0; i < mock_up_dataset->dataset->size(); i++)
    {
        if (is_center(mock_up_clusters->clusters, mock_up_dataset->dataset->at(i)->id))
        {
            continue;
        }

        ASSERT_FALSE(mock_up_dataset->dataset->at(i)->is_dirty);
        ASSERT_EQ(mock_up_dataset->dataset->at(i)->with_distance, -1);
        ASSERT_EQ(mock_up_dataset->dataset->at(i)->current_cluster, -1);
    }

    for (unsigned int i = 0; i < mock_up_clusters->clusters->size(); i++)
    {
        ASSERT_TRUE(mock_up_clusters->clusters->at(i)->centroid->is_dirty);
        ASSERT_EQ(mock_up_clusters->clusters->at(i)->data->size(), 0);
    }
}

TEST_F(ClusterInsertLloydTest, CheckIfNoReturnPoints)
{
    vector<bool> cluster_state(10, false);
    ASSERT_TRUE(cluster_insert->check_if_no_return_points(cluster_state));
}

TEST_F(ClusterInsertLloydTest, AllPointsInserted)
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