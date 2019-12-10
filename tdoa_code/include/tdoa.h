#include <bcm2835.h>
#include <stdio.h>
#include <fftw3.h>
#include <fstream>
#include <liquid/liquid.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <complex>
#include <math.h>
#include <cstring>
#include <stdlib.h>
#include <ctime>
#include"hydrophones.h"
#include <qcustomplot.h>
using namespace std;


#define ADC_CONVST_2 RPI_BPLUS_GPIO_J8_31
#define ADC_CONVST_1 RPI_BPLUS_GPIO_J8_35
#define ADC_DIN_1 RPI_BPLUS_GPIO_J8_37
#define ADC_DIN_2 RPI_BPLUS_GPIO_J8_33

//Classes

class Hydrophone{

  private:
    vector< complex<float> > tdata;
    vector< complex<float> > fdata;
    vector<int> pkind;
    static int hyd_num;


  public:
      // int len;
      int ID;
      vector<int> peaks;

      Hydrophone()
      {
        ID = hyd_num;
        //len = 0;
        hyd_num++;
      }

      void storeData(std::complex<float> x)
      {

        tdata.push_back(x);
      //  len++;
      }
      bool empty()
      {
          tdata.clear();
          return true;
      }
      void peakFinder();//finds the peak of the curve
      void calFreq();
      vector< complex<float> >  peakExtraction(int ind,int win,ofstream& file);//find the Xseg by ifft of the data
      vector< complex<float> >  peakExtraction(int ind,int win);
      void filter(float);
      void debug(int);
      void writeFile(int,char*);
      void writeFile(vector<float>,ofstream& file);

};
int Hydrophone::hyd_num = 0;



class Pair: private Hydrophone {

      static int pair_num;
      long delay;
      Hydrophone h1;
      Hydrophone h2;
      double Fs;
      void smooth();//lowpass smoothing
      int correlation(vector< complex<float> >, vector< complex<float> >,ofstream& file);//calculates the delay
      int correlation(vector< complex<float> >, vector< complex<float> >);

    public:

      int ID;
      Pair(double x)
      {
        Fs = x;
        ID = pair_num;
        pair_num++;
      }
      Pair()
      {
        Fs = 0;
        ID = pair_num;
        pair_num++;
      }
      bool flushData()
      {
        return ( h1.empty() && h2.empty());
      }

      bool getData(long);
      bool readFile(char*);
      double delay(Hydrophones*);
      double delay_modified(double*,double*, int, Hydrophones*);


};

int Pair::pair_num = 1;
