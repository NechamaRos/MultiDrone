#pragma once

#define FILTERFILE_H

#include <iostream>
#include <vector>
#include <limits>
#include <vector>
#include <array>

using namespace std;

//Class MatchKD to create an object that represents a match between 2 features of 2 images
class MatchKD {
public:
    int queryIdx;
    int trainIdx;
    float distance;

    MatchKD() : queryIdx(-1), trainIdx(-1), distance(FLT_MAX) {}

    MatchKD(int qIdx, int tIdx, float dist) : queryIdx(qIdx), trainIdx(tIdx), distance(dist) {}

    bool operator==( const MatchKD& other) const {
        return queryIdx == other.queryIdx && trainIdx == other.trainIdx && distance == other.distance;
    }

};

vector<MatchKD, allocator<MatchKD>> filterMatches(vector<vector<MatchKD>> pairMatches1, vector<vector<MatchKD>> pairMatches2);