#include"Util.h"
#include"tdoa.h"


/////////////////PAIR_FUN/////////////////////////////////
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
  // double Fs;

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
    // h1.writeFile(2,"../plots/h1fr.txt");
    // h2.writeFile(2,"../plots/h2fr.txt");

    h1.filter(Fs);
    h2.filter(Fs);
    // h2.writeFile(2,"../plots/h2filt.txt");
    // h1.writeFile(2,"../plots/h1filt.txt");

    // h1.writeFile(1,"../plots/h1tfilt.txt");
    // h2.writeFile(1,"../plots/h2tfilt.txt");
    h1.peakFinder();
    // h1.debug(4);
    // h1.writeFile(3,"../plots/h1seg.txt");
    // h1.writeFile(4,"../plots/h1peaks.txt");
    double delay = 0;

    vector< vector< complex<float> > > v1;
    for(int i=0;i<h1.peaks.size();i++)
    {
      v1.push_back(h1.peakExtraction(h1.peaks[i],5000));
    }

    vector< vector< complex<float> > > v2;
    for(int i=0;i<h1.peaks.size();i++)
    {
      v2.push_back(h2.peakExtraction(h1.peaks[i],5000)); //note here hyd1 peaks are used
    }

    vector<double> d;
    for(int i=0;i<h1.peaks.size();i++)
    {
      d.push_back(correlation(v1[i],v2[i])); //note here hyd1 peaks are used
    }
    double ans;
    double std;
    ans = mean(d);
    std = stdDev(d);
    cout<<"calculated delay:"<<ans<<endl;
    cout<<"std deviation:"<<std<<endl;

}
double Pair::correlation(vector< complex<float> > x1, vector< complex<float> > x2)
{

  std::complex<float>* input1 = x1.data();
  std::complex<float>* output1 = new std::complex<float>[x1.size()];
  fftplan fft1 = fft_create_plan(x1.size(),reinterpret_cast<liquid_float_complex*>(input1),reinterpret_cast<liquid_float_complex*>(output1), LIQUID_FFT_FORWARD, 0);
  fft_execute(fft1);


  fft_destroy_plan(fft1);

  std::complex<float>* input2 = x2.data();
  std::complex<float>* output2 = new std::complex<float>[x2.size()];
  fftplan fft2 = fft_create_plan(x2.size(),reinterpret_cast<liquid_float_complex*>(input2),reinterpret_cast<liquid_float_complex*>(output2), LIQUID_FFT_FORWARD, 0);
  fft_execute(fft2);

  fft_destroy_plan(fft2);

  vector< complex<float> > c;


  for(int i=0;i<x1.size();i++)
  {
    c.push_back((*output1++)*conj((*output2++)));
  }

  std::complex<float>* input = c.data();
  std::complex<float>* output = new std::complex<float>[c.size()];
  fftplan ifft = fft_create_plan(c.size(),reinterpret_cast<liquid_float_complex*>(input),reinterpret_cast<liquid_float_complex*>(output), LIQUID_FFT_BACKWARD, 0);
  fft_execute(ifft);

  complex<float> temp;
  vector< complex<float> > tc;
  for(int i=0; i<c.size(); i++)
  {

  temp = abs(*output++);
  tc.push_back(temp);
  }


}

void Pair::smooth()
{


}
/////////////////////HYD-FUN//////////////////////////
vector< complex<float> >  Hydrophone::peakExtraction(int x, int len)
{
      vector< complex<float> > subVec(len,0);
      complex<float> temp;
      for(int i=0; i<len;i++)
      {
        temp = abs(tdata.at(x-floor(len/2)+i));
        subVec.push_back(temp);
      }

  return subVec;
}


void Hydrophone::peakFinder()
{
  vector<float> values(0.0);

  for(int i=0;i<tdata.size();i++)
  {
    values.push_back(abs(tdata.at(i)));

  }

  std::vector<float>segment;
  int beginning = 10000;
  int ending = 10000;

  for(std::vector<float>::iterator it=values.begin()+beginning; it!=values.end()- ending; ++it)
  {
    segment.push_back(*it);
  }
  sort(segment.begin(),segment.end(),greater<float>());

  int range = 50000;
  float thr = 0;

  for(int i=0; i<range;i++)
  {
    thr += segment.at(i)/range;
  }
  cout<<thr<<endl;
  for(std::vector<float>::iterator it=values.begin()+beginning; it!=values.end()- ending; ++it)
  {
    if(*it>thr)
    pkind.push_back(1);
    else
    pkind.push_back(0);
  }
  cout<<"Thresh ends"<<endl;

  cout<<"going for wfall.. XD "<<pkind.size()<<endl;
  int win;

  vector< pair<int,int> > wfall;
  for(int i=0;i<pkind.size();i++)
  {
  		if(pkind.at(i) == 1)
  		 {   //cout<<i<<" ";
          win = 40000;
      		int count = 0;
      		int avg = 0;
          int j =0;
  				for(j= 0; (j<win && (j+i)<pkind.size());j++)
  				{
  					if(pkind.at(i+j) == 1)
  					{
  						avg += (i+j);
  						count++;

  					}

  				}
        i = i+j;
  			avg = avg/count;
  			wfall.push_back(make_pair(count,avg));
  		}
  }

  cout<<"wfall size:"<<wfall.size()<<endl;
  sort(wfall.begin(),wfall.end(),greater< pair<int,int> >());

  for(int i=0;i<7;i++)
  {
    peaks.push_back(wfall[i].second);
  }

}


void Hydrophone::writeFile(int x,char* filename)
{

ofstream file(filename);
if(file.is_open())
 {
  if(x == 1)
   {
      for(int i=0;i<tdata.size();i++)
      file<<(tdata.at(i)).real()<<endl;

      cout<<"timeSample to file done..!"<<endl;
    }
  else if(x == 2)
    {
        for(int i=0;i<fdata.size();i++)
        file<<abs(fdata.at(i))<<endl;

        cout<<"frequencySample to file done..!"<<endl;
    }
  else if(x == 3)
    {
        for(int i=0;i<pkind.size();i++)
        file<<pkind.at(i)<<endl;

        cout<<"indexSample to file done..!"<<endl;
    }
  else if(x == 4)
    {   cout<<"inside";
        for(int i=0;i<peaks.size();i++)
        file<<peaks.at(i)<<endl;

        cout<<"peaksSample to file done..!"<<endl;
    }
  else
    cout<<"wrong int value"<<endl;
 }
 else
 cout<<"file couldnot open"<<endl;

 file.close();
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
  if(i == 3)
  {
    cout<<"pkind size in hyd"<<ID<<": "<<pkind.size()<<endl;

  }
  if(i == 4)
  {
    cout<<"peaks size in hyd"<<ID<<": "<<peaks.size()<<endl;

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
  // cout<<Fs<<" "<<fdata.size()<<endl;
  if(fdata.size() != 0)
  { float b = (float)(Fs)/(float)(fdata.size());
    // cout<<b<<endl;
    // vector< complex<float> > fdata_filt(fdata.size(),0);
    int fpass1 = 77000;//remember the order fpass1<fpass2
    int fpass2 = 83000;

    int fpass3 = 37000; //remember the order fpass3<fpass4
    int fpass4 = 43000;
    for(int i=0;i<fdata.size();i++)
    {
      float freq = ((float)i*b);
      if((freq>fpass1 && freq<fpass2) ||( (freq>fdata.size()-fpass2) && (freq<fdata.size()-fpass1) ))
      continue;
      else if ((freq>fpass3 && freq<fpass4) ||( (freq>fdata.size()-fpass4) && (freq<fdata.size()-fpass3) ))
      continue;
      else
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
    tdata.at(i) = temp;
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
  // float myints[] = {1,2,3,4};
  // std::vector<float> test (myints, myints + sizeof(myints) / sizeof(int) );
  // cout<< mean(test)<<" "<<stdDev(test)<<endl;


return 0;
}
