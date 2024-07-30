#pragma once
#include <vector>
#include "MatchKD.h"

using namespace std;

vector<MatchKD, allocator<MatchKD>> filterMatches(vector<vector<MatchKD>> pairMatches1, vector<vector<MatchKD>> pairMatches2);