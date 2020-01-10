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
          fdata.clear();
          peaks.clear();
          pkind.clear();
          return true;
      }
      void peakFinder();//finds the peak of the curve
      void calFreq();
      vector< complex<float> >  peakExtraction(int ind,int win,ofstream& file);//find the Xseg by ifft of the data
      vector< complex<float> >  peakExtraction(int ind,int win);
      void filter(float);// add filter_win and center frequency
      void debug(int);
      void debug(int,Hydrophones*,int);
      void writeFile(int,char*);
      void writeFile(vector<float>,ofstream& file);
      float calPower();

};

int Hydrophone::hyd_num = 1;



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
      float getPower();

};

int Pair::pair_num = 1;

class Controller{

  static int cntrl_num;

public:
  int ID;
  float lag;
  int state; // if 1 then reading will be taken if 0 then no reading
  int gui;  // if 1 then display gui if 0 then no gui
  int mission; // if 37.5khz are executed of 45khz

  Controller()
  {
    lag = 0;
    state = 0;
    gui = 0;
    mission = 0;
  }


    Controller(int a,int b,int c)
    {
      lag = 0;
      gui = a;
      mission = b;
      state = c;
    }

  bool cntrlDelay(int); // take int btw (0-1)to calculate the delay required foro next ping to stay near center
  // bool sendToServer();
  // bool readFromServer();

};

int Controller::cntrl_num = 1;
