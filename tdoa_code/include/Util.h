// Util.h
#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#define EPS 2.2204e-16

using namespace std;

std::vector<int> smoothedZScore(std::vector<float> input,int lag ,float threshold ,float influence );
float stdDev(std::vector<float> values);
float mean(vector<float> list);

#endif
