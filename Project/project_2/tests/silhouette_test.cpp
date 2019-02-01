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
#include "../evaluation/silhouette.hpp"

namespace
{

class SilhouetteTest : public testing::Test
{
  protected:
    SilhouetteTest()
    {
    }

    ~SilhouetteTest() override
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
        silhouette = new Silhouette<double>();
    }

    void TearDown() override
    {
        delete mock_up_dataset;
        delete mock_up_clusters;
        delete cluster_insert;
        delete silhouette;
        delete data_manager;
    }

    MockUpDataset<double> *mock_up_dataset;
    MockUpClusters<double> *mock_up_clusters;
    ClusterInsert<double> *cluster_insert;
    Silhouette<double> *silhouette;
    DataManager<double> *data_manager;
};

TEST_F(SilhouetteTest, GetNearestCluster)
{
    int result_index = silhouette->get_nearest_cluster(mock_up_clusters->clusters, mock_up_dataset->eu_metrics, 0);
    double result_distance = mock_up_dataset->eu_metrics->get_distance(
        mock_up_clusters->clusters->at(result_index)->centroid->vector_data,
        mock_up_clusters->clusters->at(0)->centroid->vector_data);

    ASSERT_LE(
        result_distance,
        mock_up_dataset->eu_metrics->get_distance(
            mock_up_clusters->clusters->at(0)->centroid->vector_data,
            mock_up_clusters->clusters->at(result_index == 1 ? 2 : 1)->centroid->vector_data));
}

TEST_F(SilhouetteTest, GetClusterAverage)
{
    for (unsigned int i = 0; i < mock_up_clusters->clusters->size(); i++)
    {
        if (mock_up_clusters->clusters->at(i)->data->size() != 0)
        {
            double avg = silhouette->get_cluster_average(
                mock_up_clusters->clusters->at(i),
                mock_up_clusters->clusters->at(i)->data->at(0),
                mock_up_dataset->eu_metrics);
            ASSERT_NE(avg, 0);
            break;
        }
    }
}

TEST_F(SilhouetteTest, Evaluate)
{
    silhouette->evaluate(mock_up_clusters->clusters,
                         mock_up_dataset->dataset,
                         mock_up_dataset->eu_metrics);

    for (unsigned int i = 0; i < mock_up_dataset->dataset->size(); i++)
    {
        ASSERT_NE(mock_up_dataset->dataset->at(i)->silhouette, -2);
        ASSERT_LT(mock_up_dataset->dataset->at(i)->silhouette, 1);
        ASSERT_GT(mock_up_dataset->dataset->at(i)->silhouette, -1);
    }
}

} // namespace