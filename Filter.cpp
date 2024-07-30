
#include "Filter.h"
#include "MatchKD.h"
#include <set> 

/*
* Filters the matches by the distances (->Ratio-test), and by using the A->B algorithm.
* 
* @param pairMatches1 - The vector which presents pair of matches between image A to image B.
* @param pairMatches2 - The vector which presents pair of matches between image B to image A.
* 
* return the vector of filtered matches according to the ratio-test, 
* or empty; if pairMatches was empty or if filtered-matches were not found.
*/

vector<MatchKD, allocator<MatchKD>> filterMatches(vector<vector<MatchKD>> pairMatches1, vector<vector<MatchKD>> pairMatches2) {

    float matchConf = 0.3f;
    vector<MatchKD, allocator<MatchKD>> matches;
    set<pair<int, int>> tempmatch;

    // Finding filtered matches from A to B
    for (size_t i = 0; i < pairMatches1.size(); ++i)
    {
        // Checking if there are less than 2 matches
        if (pairMatches1[i].size() < 2)
            continue;
        const MatchKD& m0 = pairMatches1[i][0];
        const MatchKD& m1 = pairMatches1[i][1];
        // Ratio-test
        if (m0.distance < (1.f - matchConf) * m1.distance)
        {
            matches.push_back(m0);
            tempmatch.insert(std::make_pair(m0.queryIdx, m0.trainIdx));
        }
    }

    // Finding filtered matches B to A
    for (size_t i = 0; i < pairMatches2.size(); ++i)
    {
        // Checking if there are less than 2 matches
        if (pairMatches2[i].size() < 2)
            continue;
        const MatchKD& m0 = pairMatches2[i][0];
        const MatchKD& m1 = pairMatches2[i][1];
        // Ratio-test
        if (m0.distance < (1.f - matchConf) * m1.distance)
            if (tempmatch.find(std::make_pair(m0.trainIdx, m0.queryIdx)) == tempmatch.end())
                matches.push_back(MatchKD(m0.trainIdx, m0.queryIdx, m0.distance));
    }
    return matches;
}