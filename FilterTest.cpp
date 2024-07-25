#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "FilterFile.h"
#include <iostream>
#include <vector>
#include <limits>
#include <vector>
#include <array>


bool areVectorsEqual(const std::vector<MatchKD>& vec1, const std::vector<MatchKD>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (!(vec1[i] == vec2[i])) {
            return false;
        }
    }
    return true;
}

void printMatches(const std::vector<MatchKD>& matches) {
    for (const auto& match : matches) {
        std::cout << "QueryIdx: " << match.queryIdx << ", TrainIdx: " << match.trainIdx << ", Distance: " << match.distance << std::endl;
    }
}

TEST_CASE("testing the filterMatches function when there is a match") {

    vector<vector<MatchKD>> pairMatches1 = {
       {MatchKD(0, 191, 77.0f), MatchKD(0, 27, 82.0f)},
       {MatchKD(1, 18, 46.0f), MatchKD(1, 46, 69.0f)},
       {MatchKD(2, 163, 98.0f), MatchKD(2, 139, 102.0f)},
       {MatchKD(3, 89, 64.0f), MatchKD(3, 220, 73.0f)},
    };

    vector<vector<MatchKD>> pairMatches2 = {
       {MatchKD(0, 321, 70.0f), MatchKD(0, 164, 78.0f)},
       {MatchKD(1, 50, 65.0f), MatchKD(1, 166, 76.0f)},
       {MatchKD(2, 360, 86.0f), MatchKD(2, 48, 88.0f)},
       {MatchKD(3, 107, 79.0f), MatchKD(3, 196, 85.0f)},
    };

    vector<MatchKD> expectedMatches = {
        MatchKD(1, 18, 46.0f)
    };

    vector<MatchKD> actualMatches = filterMatches(pairMatches1, pairMatches2);

    printMatches(actualMatches);
    CHECK(areVectorsEqual(actualMatches, expectedMatches));

}

TEST_CASE("Testing empty inputs") {
    vector<vector<MatchKD>> empty1, empty2;
    vector<MatchKD> expectedMatches;
    vector<MatchKD> actualMatches = filterMatches(empty1, empty2);

    std::cout << "Testing empty inputs - Expected: 0 matches, Actual: " << actualMatches.size() << " matches" << std::endl;
    CHECK(areVectorsEqual(actualMatches, expectedMatches));
}



TEST_CASE("Tesing filterMatches when filtered not found")
{
    std::vector<std::vector<MatchKD>> pairMatches1 = {
       {MatchKD(0, 191, 77.0f), MatchKD(0, 27, 82.0f)},
       {MatchKD(2, 163, 98.0f), MatchKD(2, 139, 102.0f)},
       {MatchKD(3, 89, 64.0f), MatchKD(3, 220, 73.0f)},
    };

    std::vector<std::vector<MatchKD>> pairMatches2 = {
       {MatchKD(0, 191, 77.0f), MatchKD(0, 27, 82.0f)},
       {MatchKD(2, 163, 98.0f), MatchKD(2, 139, 102.0f)},
       {MatchKD(3, 89, 64.0f), MatchKD(3, 220, 73.0f)},
    };

    vector<MatchKD> actualMatches = filterMatches(pairMatches1, pairMatches2);
    vector<MatchKD> expectedMatches;

    std::cout << "Tesing filterMatches when filtered not found: " << actualMatches.size() << " matches" << std::endl;
    CHECK(areVectorsEqual(actualMatches, expectedMatches));
}