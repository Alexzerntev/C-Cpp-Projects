#include "gtest/gtest.h"
#include "./mockups/mockup_dataset.hpp"
#include "../common/data_manipulation/data_manager.hpp"
#include "../common/data_manipulation/data_item.hpp"
#include "../common/random_generator.hpp"
#include "../common/metrics.hpp"
#include "../common/enums.hpp"
#include "../clustering/cluster_initialize.hpp"

namespace
{

// The fixture for testing class Foo.
class ClusterInitializeTest : public testing::Test
{
  protected:
    ClusterInitializeTest()
    {
    }

    ~ClusterInitializeTest() override
    {
    }

    void SetUp() override
    {
        mock_up_dataset = new MockUpDataset<double>();
        cluster_initialize = new ClusterInitialize<double>();
    }

    void TearDown() override
    {
        delete mock_up_dataset;
        delete cluster_initialize;
    }
    MockUpDataset<double> *mock_up_dataset;
    ClusterInitialize<double> *cluster_initialize;
};

TEST_F(ClusterInitializeTest, GetMinDistanceEuclidean)
{
    double min_distance1 = cluster_initialize->get_min_distance(mock_up_dataset->eu_metrics, mock_up_dataset->dataset, 0);
    double min_distance2 = cluster_initialize->get_min_distance(mock_up_dataset->eu_metrics, mock_up_dataset->dataset, 0);
    ASSERT_NEAR(min_distance1, sqrt(2), 3);
    ASSERT_DOUBLE_EQ(min_distance1, min_distance2);
}

TEST_F(ClusterInitializeTest, GetMinDistanceCosine)
{
    double min_distance1 = cluster_initialize->get_min_distance(mock_up_dataset->cos_metrics, mock_up_dataset->dataset, 0);
    double min_distance2 = cluster_initialize->get_min_distance(mock_up_dataset->cos_metrics, mock_up_dataset->dataset, 0);
    double min_distance3 = cluster_initialize->get_min_distance(mock_up_dataset->cos_metrics, mock_up_dataset->dataset, 1);
    ASSERT_NEAR(min_distance3, 1, 3);
    ASSERT_DOUBLE_EQ(min_distance1, min_distance2);
}

TEST_F(ClusterInitializeTest, InitializeClusterWithRandom2ClustersEuclidean)
{
    vector<Cluster<double> *> *clusters = cluster_initialize->initialize_cluster(2, mock_up_dataset->dataset, RANDOM_INITIALIZATION, mock_up_dataset->eu_metrics);
    ASSERT_EQ(clusters->size(), 2);
    ASSERT_DOUBLE_EQ(clusters->at(0)->index, 0);
    ASSERT_DOUBLE_EQ(clusters->at(1)->index, 1);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->with_distance, -1);
    ASSERT_NE(clusters->at(0)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(1)->centroid->current_cluster, -1);
    Cluster<double> *temp;
    while (clusters->size() > 0)
    {
        temp = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp;
    }
    delete clusters;
}

TEST_F(ClusterInitializeTest, InitializeClusterWithRandom2ClustersCosine)
{
    vector<Cluster<double> *> *clusters = cluster_initialize->initialize_cluster(2, mock_up_dataset->dataset, RANDOM_INITIALIZATION, mock_up_dataset->cos_metrics);
    ASSERT_EQ(clusters->size(), 2);
    ASSERT_DOUBLE_EQ(clusters->at(0)->index, 0);
    ASSERT_DOUBLE_EQ(clusters->at(1)->index, 1);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->with_distance, -1);
    ASSERT_NE(clusters->at(0)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(1)->centroid->current_cluster, -1);
    Cluster<double> *temp;
    while (clusters->size() > 0)
    {
        temp = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp;
    }
    delete clusters;
}

TEST_F(ClusterInitializeTest, InitializeClusterWithKMeans2ClustersEuclidean)
{
    vector<Cluster<double> *> *clusters = cluster_initialize->initialize_cluster(2, mock_up_dataset->dataset, KMEANS_PP_INITIALIZATION, mock_up_dataset->eu_metrics);
    ASSERT_EQ(clusters->size(), 2);
    ASSERT_DOUBLE_EQ(clusters->at(0)->index, 0);
    ASSERT_DOUBLE_EQ(clusters->at(1)->index, 1);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->with_distance, -1);
    ASSERT_NE(clusters->at(0)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(1)->centroid->current_cluster, -1);
    Cluster<double> *temp;
    while (clusters->size() > 0)
    {
        temp = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp;
    }
    delete clusters;
}

TEST_F(ClusterInitializeTest, InitializeClusterWithKMeans2ClustersCosine)
{
    vector<Cluster<double> *> *clusters = cluster_initialize->initialize_cluster(2, mock_up_dataset->dataset, KMEANS_PP_INITIALIZATION, mock_up_dataset->cos_metrics);
    ASSERT_EQ(clusters->size(), 2);
    ASSERT_DOUBLE_EQ(clusters->at(0)->index, 0);
    ASSERT_DOUBLE_EQ(clusters->at(1)->index, 1);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->with_distance, -1);
    ASSERT_NE(clusters->at(0)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(1)->centroid->current_cluster, -1);
    Cluster<double> *temp;
    while (clusters->size() > 0)
    {
        temp = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp;
    }
    delete clusters;
}

TEST_F(ClusterInitializeTest, InitializeClusterWithRandom3ClustersEuclidean)
{
    vector<Cluster<double> *> *clusters = cluster_initialize->initialize_cluster(3, mock_up_dataset->dataset, RANDOM_INITIALIZATION, mock_up_dataset->eu_metrics);
    ASSERT_EQ(clusters->size(), 3);
    ASSERT_DOUBLE_EQ(clusters->at(0)->index, 0);
    ASSERT_DOUBLE_EQ(clusters->at(1)->index, 1);
    ASSERT_DOUBLE_EQ(clusters->at(2)->index, 2);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(2)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(2)->centroid->with_distance, -1);
    ASSERT_NE(clusters->at(0)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(1)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(2)->centroid->current_cluster, -1);
    Cluster<double> *temp;
    while (clusters->size() > 0)
    {
        temp = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp;
    }
    delete clusters;
}

TEST_F(ClusterInitializeTest, InitializeClusterWithRandom3ClustersCosine)
{
    vector<Cluster<double> *> *clusters = cluster_initialize->initialize_cluster(3, mock_up_dataset->dataset, RANDOM_INITIALIZATION, mock_up_dataset->cos_metrics);
    ASSERT_EQ(clusters->size(), 3);
    ASSERT_DOUBLE_EQ(clusters->at(0)->index, 0);
    ASSERT_DOUBLE_EQ(clusters->at(1)->index, 1);
    ASSERT_DOUBLE_EQ(clusters->at(2)->index, 2);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(2)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(2)->centroid->with_distance, -1);
    ASSERT_NE(clusters->at(0)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(1)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(2)->centroid->current_cluster, -1);
    Cluster<double> *temp;
    while (clusters->size() > 0)
    {
        temp = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp;
    }
    delete clusters;
}

TEST_F(ClusterInitializeTest, InitializeClusterWithKMeans3ClustersEuclidean)
{
    vector<Cluster<double> *> *clusters = cluster_initialize->initialize_cluster(3, mock_up_dataset->dataset, KMEANS_PP_INITIALIZATION, mock_up_dataset->eu_metrics);
    ASSERT_EQ(clusters->size(), 3);
    ASSERT_DOUBLE_EQ(clusters->at(0)->index, 0);
    ASSERT_DOUBLE_EQ(clusters->at(1)->index, 1);
    ASSERT_DOUBLE_EQ(clusters->at(2)->index, 2);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(2)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(2)->centroid->with_distance, -1);
    ASSERT_NE(clusters->at(0)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(1)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(2)->centroid->current_cluster, -1);
    Cluster<double> *temp;
    while (clusters->size() > 0)
    {
        temp = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp;
    }
    delete clusters;
}

TEST_F(ClusterInitializeTest, InitializeClusterWithKMeans3ClustersCosine)
{
    vector<Cluster<double> *> *clusters = cluster_initialize->initialize_cluster(3, mock_up_dataset->dataset, KMEANS_PP_INITIALIZATION, mock_up_dataset->cos_metrics);
    ASSERT_EQ(clusters->size(), 3);
    ASSERT_DOUBLE_EQ(clusters->at(0)->index, 0);
    ASSERT_DOUBLE_EQ(clusters->at(1)->index, 1);
    ASSERT_DOUBLE_EQ(clusters->at(2)->index, 2);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(2)->centroid->is_dirty, true);
    ASSERT_DOUBLE_EQ(clusters->at(0)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(1)->centroid->with_distance, -1);
    ASSERT_DOUBLE_EQ(clusters->at(2)->centroid->with_distance, -1);
    ASSERT_NE(clusters->at(0)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(1)->centroid->current_cluster, -1);
    ASSERT_NE(clusters->at(2)->centroid->current_cluster, -1);
    Cluster<double> *temp;
    while (clusters->size() > 0)
    {
        temp = clusters->at(0);
        clusters->erase(clusters->begin());
        delete temp;
    }
    delete clusters;
}
} // namespace