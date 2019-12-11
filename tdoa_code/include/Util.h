// Util.h
#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <complex>

#define EPS 2.2204e-16

using namespace std;

double stdDev(std::vector<int> values);
double mean(vector<int> list);
void lrotateByOne(vector<float>& v);
void lrotateByOne(vector<float>& v, long n);
void convtDouble(vector< complex<float> >,int,double*);
void convtDouble(vector<int>,int,double*);
// add writeFile && readFile here
// add fft and ifft functions here

#endif
