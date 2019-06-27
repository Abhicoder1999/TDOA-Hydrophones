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


//Functions

void Hydrophone::debug()
{

  tdata.pop_back();
  cout<<"datasize in hyd"<<ID<<": "<<tdata.size()<<endl;
  // len = len-1;
  // cout<<"data in hyd"<<ID<<" len:"<<len<<endl;
  // cout<<tdata.at(0)<<" "<<tdata.at(len-2)<<tdata.at(len-1)<<endl;


}

bool Pair::readFile(char* filename)
{
  ifstream file;
  double Fs;

  std::complex<float> temp;

  file.open(filename);
  if(file.is_open())
  {
    file>>Fs;
    cout<<Fs<<endl;
    float data;
    while(!file.eof())
    {

      file>>data;
      temp = data;
      h1.storeData(temp);
      // cout<<data<<endl;


      file>>data;
      temp = data;
      h2.storeData(temp);
      // cout<<data<<endl;
    }

    // cout<<"file closed"<<endl;
    file.close();

    h1.debug();
    h2.debug();
    return true;
  }
  else
  {
    cout<<"file didnt open"<<endl;
    return false;
  }

}



void Hydrophone::calFreq()
{

  fftplan fft = fft_create_plan(tdata.size(),reinterpret_cast<liquid_float_complex*>(&tdata),reinterpret_cast<liquid_float_complex*>(&fdata), LIQUID_FFT_FORWARD, 0);
  fft_execute(fft);
  fft_destroy_plan(fft);

}

void Hydrophone::filter(float Fs)
{

  if(fdata.size() != 0)
  { float temp = Fs/(float)(fdata.size());//check if this is same as tdata
    for(int i=0;i<fdata.size();i++)
    {
      float freq = (float)(i*Fs)/(float)(fdata.size());
      if( freq<38000  &&  freq>42000 || freq<78000  &&  freq>82000 )
      fdata.at(i) = 0;
    }

    fftplan ifft = fft_create_plan(tdata.size(),reinterpret_cast<liquid_float_complex*>(&fdata),reinterpret_cast<liquid_float_complex*>(&tdata), LIQUID_FFT_BACKWARD, 0);
    fft_execute(ifft);
    fft_destroy_plan(ifft);

  }
  else
  cout<<"frequency domain khali..!";

}


bool Pair::getData(long len)
{

      if (!bcm2835_init()){  printf("bcm2835_init failed.\n");
        	return(0);}
    	if (!bcm2835_spi_begin()){ printf("bcm2835_spi_begin failed.\n");
		      return(0);}

    	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);
    	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);
    	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
    	bcm2835_gpio_fsel(ADC_DIN_1, BCM2835_GPIO_FSEL_OUTP);
    	bcm2835_gpio_fsel(ADC_DIN_2, BCM2835_GPIO_FSEL_OUTP);
	    bcm2835_gpio_fsel(ADC_CONVST_1, BCM2835_GPIO_FSEL_OUTP);
	    bcm2835_gpio_fsel(ADC_CONVST_2, BCM2835_GPIO_FSEL_OUTP);

	    float 	adcbinarydata;//
	    float 	adcvoltage;
	    // len = 500000;
      uint8_t read_data[len];
	    uint8_t read_data1[len];
	    uint8_t read_data2[len];

	    uint8_t read[len];
    	uint8_t read1[len];
    	uint8_t read2[len];

    	clock_t begin_t;
    	clock_t clk_check1;

    	bcm2835_gpio_write(ADC_DIN_1, LOW);
    	bcm2835_gpio_write(ADC_DIN_2, LOW);
    	bcm2835_gpio_write(ADC_CONVST_1, LOW);
		  bcm2835_gpio_write(ADC_CONVST_2, LOW);

    	begin_t = clock();

    	bcm2835_gpio_write(ADC_DIN_1, HIGH);
    	bcm2835_gpio_write(ADC_CONVST_1, HIGH);
    	bcm2835_gpio_write(ADC_DIN_2, HIGH);
			bcm2835_delayMicroseconds(1);

    	for(int i=0; i<len; i++)
      {

          		bcm2835_gpio_write(ADC_CONVST_2, HIGH);
          		bcm2835_gpio_write(ADC_DIN_1, LOW);


              read_data[i]  = bcm2835_spi_transfer(0);
              read_data1[i] = bcm2835_spi_transfer(0);
              read_data2[i] = bcm2835_spi_transfer(0);

          		bcm2835_gpio_write(ADC_CONVST_1, LOW);
          		bcm2835_gpio_write(ADC_DIN_1, HIGH);
          		bcm2835_gpio_write(ADC_CONVST_1, HIGH);
             	bcm2835_gpio_write(ADC_DIN_2, LOW);


              read[i]  = bcm2835_spi_transfer(0);
              read1[i] = bcm2835_spi_transfer(0);
              read2[i] = bcm2835_spi_transfer(0);


          		bcm2835_gpio_write(ADC_CONVST_2, LOW);
          		bcm2835_gpio_write(ADC_DIN_2, HIGH);

	          }
      cout << (CLOCKS_PER_SEC*(float)len/(clock() - begin_t)) << "Hz" << endl;
      Fs = CLOCKS_PER_SEC*(float)len/(clock() - begin_t);//


      std::complex<float> temp;//
      for(int i=0; i<len; i++)
    	{

      	read_data[i] = read_data[i] & 0x7F;
    		read_data2[i] = (read_data2[i] >> 6) & 0x03;
    		adcbinarydata = ((uint32_t)read_data[i] << 10) | ((uint32_t)read_data1[i] << 2) | (read_data2[i]);
        temp = adcbinarydata;//
        h1.storeData(temp);//

    		read[i] = read[i] & 0x7F;
    		read2[i] = (read2[i] >> 6) & 0x03;
    		adcbinarydata = ((uint32_t)read[i] << 10) | ((uint32_t)read1[i] << 2) | (read2[i]);
        temp = adcbinarydata;//
        h2.storeData(temp);//

      }
        	bcm2835_spi_end();
        	bcm2835_close();
}



int main()
{
  char* filename = "../pinger_data/l0.txt";
  // cout<<filename<<endl;
  Pair p1;
//  cout<<p1.ID<<" "<<p1.h1.ID<<" "<<p1.h2.ID<<endl;
  if(p1.readFile(filename))
  cout<<"data read from file successfully"<<endl;
  else
  cout<<"file couldnot be read"<<endl;

return 0;
}
