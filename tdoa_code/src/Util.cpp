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

float mean(vector<float> list)
{
		float sum = 0;
    for(int i=0;i<list.size();i++)
		sum += list.at(i);

		return sum/(float)list.size();
}


float stdDev(std::vector<float> values)
{
		float ret = 0;
		if (values.size() > 0)
		{
				float avg = mean(values);
				float sd = 0;
				for(int i=0;i<values.size();i++)
				sd += pow((values.at(i) - avg)/pow(10,8) ,2);

				sd = sqrt(sd)/((values.size()-1));
				return sd;
		}

}

std::vector<int> smoothedZScore(std::vector<float> input,int lag,float threshold,float influence)
{

    if (input.size() <= lag + 2)
    {
        std::vector<int> emptyVec;
        return emptyVec;
				cout<<"list is too small here"<<endl;
    }

    //Initialise variables
    std::vector<int> signals(input.size(), 0.0);
    std::vector<float> filteredY(input.size(), 0.0);
    std::vector<float> avgFilter(input.size(), 0.0);
    std::vector<double> stdFilter(input.size(), 0.0);
    std::vector<float> subVecStart(input.begin(), input.begin() + lag);
    avgFilter[lag] = mean(subVecStart);
    stdFilter[lag] = stdDev(subVecStart);
		cout<<stdFilter[lag]<<endl;
		// int countP = 0;
		// int countN = 0;
		// int zero = 0;
    for (int i = lag + 1; i < input.size(); i++) //size_t i tha changed to int
    {		//cout<<abs(input[i] - avgFilter[i - 1])<<" "<<threshold * stdFilter[i - 1]<<endl;
        if (float(abs(input[i] - avgFilter[i - 1])) > threshold * stdFilter[i - 1])
        {
            // if (input[i] > avgFilter[i - 1])
            // {		countP++;
            //     cc //# Positive signal
            // }
            // else
						// {
						// 		countN++;
            //     signals[i] = -1; //# Negative signal
            // }
						signals[i] = 1;
            //Make influence lower
            filteredY[i] = influence* input[i] + (1 - influence) * filteredY[i - 1];
        }
        else
        {
            signals[i] = 0; //# No signal
            filteredY[i] = input[i];
        }
        //Adjust the filters
        std::vector<float> subVec(filteredY.begin() + i - lag, filteredY.begin() + i);
        avgFilter[i] = mean(subVec);
        stdFilter[i] = stdDev(subVec);
    }

		// cout<<threshold<<" "<<countP<<" "<<countN<<" "<<zero<<endl;
    return signals;
}
