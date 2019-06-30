// Util.h
#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#define EPS 2.2204e-16

using namespace std;

double stdDev(std::vector<int> values);
double mean(vector<int> list);
void lrotateByOne(vector<float>& v);
void lrotateByOne(vector<float>& v, long n);

#endif
