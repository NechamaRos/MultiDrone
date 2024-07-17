#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <stdint.h>
#include <opencv2/opencv.hpp>
#include <any>
#include <map>
#include "HashTable.h"
#include <algorithm>

// for each point from what img and the poin distance distance
struct PointMatch
{
	int queryIdx;
	int trainIdx;
	float distance;
};

//Search behavior, such as speed, accuracy and order of results.
struct SearchParams
{
	int checks = 32;
	float eps = 0;
	bool sorted = true;
};
class MatchFeaturers
{
private:
    std::vector<PointMatch>good_matches;
    std::vector<std::vector<PointMatch>> knn_matches;
	//what hash table?
	typedef int DataType;
	// What is stored in the hash 
	typedef uint32_t Featuretype;
	// Id type from which we can get a bucket back in an hash table
	typedef unsigned int BucketType;
	//a bucken in a hash table
	typedef std::vector<uint32_t> Bucket;
public:
    
	std::vector<PointMatch> knnMatch(cv::Mat descriptors1, cv::Mat descriptors2,
	std::vector<PointMatch> knn_matches, int k);
	void knnSearch(cv::Mat descriptors1, cv::Mat descriptors2,cv::Mat& indices,cv::Mat& dists,int knn, SearchParams searchParams);
    std::vector<PointMatch> matchFilter(std::vector<std::vector<PointMatch>> knn_matches);
	//find naerst neighbors for point returns map<indx,distance>
   void findNeighbors();
};