#include"tdoa.h"
#include"Util.h"

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

    h1.debug(1);
    h2.debug(1);
    return true;
  }
  else
  {
    cout<<"file didnt open"<<endl;
    return false;
  }
}

void Pair::delay()
{

    h1.calFreq();
    h2.calFreq();

    h1.debug(2);
    h2.debug(2);

    h1.writeFile(2,"../plots/h1fr.txt");
    h2.writeFile(2,"../plots/h2fr.txt");
    h1.filter(Fs);
    h2.filter(Fs);

    h1.debug(2);
    h2.debug(2);

}

void Hydrophone::writeFile(int x,char* filename)
{

ofstream file(filename);
if(file.is_open())
 {
  if(x == 1)
   {
      for(int i=0;i<tdata.size();i++)
      file<<tdata.at(i)<<endl;

      cout<<"timeSample to file done..!"<<endl;
    }
  if(x == 2)
    {
        for(int i=0;i<fdata.size();i++)
        file<<abs(fdata.at(i))<<endl;

        cout<<"frequencySample to file done..!"<<endl;
    }
 }
}

void Hydrophone::debug(int i)
{
  if(i == 1)
  {
    tdata.pop_back();
    cout<<"datasize in hyd"<<ID<<": "<<tdata.size()<<endl;
  }
  if(i == 2)
  {
    cout<<"frq size in hyd"<<ID<<": "<<fdata.size()<<endl;
    // cout<<"first and last element: "<<abs(fdata.at(0))<<" "<<abs(fdata.at(fdata.size()-1))<<endl;
  }
}



void Hydrophone::calFreq()
{
  // float complex * y = (float complex*) malloc(tdata.size() * sizeof(float complex));
  std::complex<float>* input = tdata.data();
  std::complex<float>* output = new std::complex<float>[tdata.size()];
  fftplan fft = fft_create_plan(tdata.size(),reinterpret_cast<liquid_float_complex*>(input),reinterpret_cast<liquid_float_complex*>(output), LIQUID_FFT_FORWARD, 0);
  fft_execute(fft);
  complex<float> temp;
  for(int i=0; i<tdata.size(); i++)
  {
  // cout<<abs(*output++)<<" ";
  temp = abs(*output++);
  fdata.push_back(temp);
  }
  fft_destroy_plan(fft);
  cout<<"fft done successfully :) for hyd"<<ID<<endl;

}

void Hydrophone::filter(float Fs)
{

  if(fdata.size() != 0)
  { float b = Fs/(float)(fdata.size());//check if this is same as tdata
    for(int i=0;i<fdata.size();i++)
    {
      float freq = (float)i*b;
      if( freq<38000  &&  freq>42000 || freq<78000  &&  freq>82000 )
      fdata.at(i) = 0;
    }

    std::complex<float>* input = fdata.data();
    std::complex<float>* output = new std::complex<float>[fdata.size()];
    fftplan ifft = fft_create_plan(tdata.size(),reinterpret_cast<liquid_float_complex*>(input),reinterpret_cast<liquid_float_complex*>(output), LIQUID_FFT_BACKWARD, 0);
    fft_execute(ifft);

    complex<float> temp;
    for(int i=0; i<fdata.size(); i++)
    {
    // cout<<abs(*output++)<<" ";
    temp = abs(*output++);
    tdata.push_back(temp);
    }

    cout<<"filtering Done for hyd"<<ID<<endl;
    fft_destroy_plan(ifft);


  }
  else
  cout<<"frequency domain khali..!";

}


int main()
{
  char* filename = "../pinger_data/l0.txt";
  // cout<<filename<<endl;
  Pair p1;
//  cout<<p1.ID<<" "<<p1.h1.ID<<" "<<p1.h2.ID<<endl;
  if(p1.readFile(filename))
  {
    cout<<"data read from file successfully"<<endl;
  }
  else
  cout<<"file couldnot be read"<<endl;

  p1.delay();

return 0;
}
