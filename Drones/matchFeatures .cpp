#include "matchFeatures .h"

using namespace std;
std::vector<PointMatch> MatchFeaturers::knnMatch(cv::Mat descriptors1, cv::Mat descriptors2, std::vector<PointMatch> knn_matches, int k)
{
    //for storing the neighbors
    cv::Mat indices(descriptors1.size(), k,1);
    //for storing neighbors distance 
    cv::Mat dists(descriptors1.size(), k, 1);
    //mask to help accuracy and efficiency of the algorithm
    std::vector<int>mask(descriptors1.rows, 0);
    SearchParams searchParams;

    knnSearch(descriptors1, descriptors2, indices, dists, k, searchParams);
    //storing and sorting the indices and dist in point obj for knn_matches
    for (int i = 0; i < descriptors1.rows; i++)
    {
        std::vector<PointMatch>matches;
            for (int j = 0; j < k; j++)
            {
                PointMatch point;
                point.queryIdx = i;
                point.trainIdx = *(indices.ptr(i,j));
                point.distance = *(dists.ptr(i, j));
                matches.push_back(point);
            }
            std::sort(matches.begin(), matches.end(), [](const PointMatch& a, const PointMatch& b) {
                return a.distance < b.distance; });

            knn_matches.push_back(matches.at(i));
            matches.clear();
    }
    return knn_matches;
}

void MatchFeaturers::knnSearch(cv::Mat descriptors1, cv::Mat descriptors2, cv::Mat& indices, cv::Mat& dists, int knn, SearchParams searchParams)
{
    ResultSet<float> resultSet(knn);
    ////for (int i = 0; i < descriptors1.rows; i++) {
    ////    resultSet.clear();
    ////    std::fill_n(indices.at<int>(i), knn, -1);
    ////    std::fill_n(dists.at<int>(i), knn, -1);
    ////    std::map<int, float> neighbor;
    ////    findNeighbors(resultSet, descriptors1, searchParams);
    ////    if (get_param(params, "sorted", true)) resultSet.sortAndCopy(indices[i], dists[i], knn);
    ////    else resultSet.copy(indices[i], dists[i], knn);
    ////}
    
}

std::vector<PointMatch> MatchFeaturers::matchFilter(std::vector<std::vector<PointMatch>> knn_matches)
{
    for (int i = 0; i < knn_matches.size(); ++i)
    {
        if (knn_matches[i][0].distance < 0.7 * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }
    return good_matches;
}

void MatchFeaturers::findNeighbors()
{
    
    vector<HashTable<DataType>> tables;
    vector<HashTable<DataType>> ::const_iterator table = tables.begin();
    vector<HashTable<DataType>>::const_iterator table_end = tables.end();
    //iterator on table
  //  HashTable<DataType> myTable;

    for (; table != table_end; ++table) {
        size_t key = 0;// tables.getKey(vec);
        vector<BucketType>xor_masks;
        vector<BucketType>::const_iterator xor_mask = xor_masks.begin();
        vector<BucketType>::const_iterator xor_mask_end = xor_masks.end();

        for (; xor_mask != xor_mask_end; ++xor_mask) {
            size_t sub_key = key ^ (*xor_mask);
            const Bucket* bucket = 0;// table->getBucketFromKey(sub_key);
            if (bucket == 0) continue;

            // Go over each descriptor index
            vector<Featuretype>::const_iterator training_index = bucket->begin();
            std::vector<Featuretype>::const_iterator last_training_index = bucket->end();
            float _distance;

            // running on the bucket->the discrip2
            for (; training_index < last_training_index; ++training_index) {
                // Compute the Hamming distance
           /*     _distance = distance_(vec, dataset_[*training_index], (int)dataset_.cols);
                result.addPoint(hamming_distance, *training_index);*/
            }
        }
    }
}
