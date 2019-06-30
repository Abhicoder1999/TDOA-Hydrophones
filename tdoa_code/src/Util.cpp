// Util.cpp
#include "Util.h"


void diff(vector<float> in, vector<float>& out)
{
	out = vector<float>(in.size()-1);

	for(int i=1; i<in.size(); ++i)
		out[i-1] = in[i] - in[i-1];
}

void vectorProduct(vector<float> a, vector<float> b, vector<float>& out)
{
	out = vector<float>(a.size());

	for(int i=0; i<a.size(); ++i)
		out[i] = a[i] * b[i];
}

void findIndicesLessThan(vector<float> in, float threshold, vector<int>& indices)
{
	for(int i=0; i<in.size(); ++i)
		if(in[i]<threshold)
			indices.push_back(i+1);
}

void selectElements(vector<float> in, vector<int> indices, vector<float>& out)
{
	for(int i=0; i<indices.size(); ++i)
		out.push_back(in[indices[i]]);
}

void selectElements(vector<int> in, vector<int> indices, vector<int>& out)
{
	for(int i=0; i<indices.size(); ++i)
		out.push_back(in[indices[i]]);
}

void signVector(vector<float> in, vector<int>& out)
{
	out = vector<int>(in.size());

	for(int i=0; i<in.size(); ++i)
	{
		if(in[i]>0)
			out[i]=1;
		else if(in[i]<0)
			out[i]=-1;
		else
			out[i]=0;
	}
}

double mean(vector<int> list)
{
		double sum = 0;
    for(int i=0;i<list.size();i++)
		sum += list.at(i);

		return sum/(double)list.size();
}


double stdDev(std::vector<int> values)
{
		double ret = 0;
		if (values.size() > 0)
		{
				double avg = mean(values);
				double sd = 0;
				for(int i=0;i<values.size();i++)
				sd += pow((values.at(i) - avg) ,2);

				sd = sqrt(sd)/((values.size()-1));
				return sd;
		}

}
