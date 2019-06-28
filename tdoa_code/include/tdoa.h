#include <bcm2835.h>
#include <stdio.h>
#include <fstream>
#include <liquid/liquid.h>
#include <iostream>
#include <vector>
#include <complex>
#include <math.h>
#include <cstring>
#include<stdlib.h>
#include <ctime>
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
    static int hyd_num;
  public:
      // int len;
      int ID;

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
      void peakFinder();//finds the peak of the curve
      void calFreq();
      void peakExtraction();//find the Xseg by ifft of the data recieved
      void filter(float);
      void debug();

};
int Hydrophone::hyd_num = 0;



class Pair: public Hydrophone {

      static int pair_num;
      long delay;

    public:
      Hydrophone h1;
      Hydrophone h2;

      double Fs;
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
      bool getData(long);
      bool readFile(char*);
      void delay();

};

int Pair::pair_num = 1;
