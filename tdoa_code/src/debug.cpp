#include"tdoa.h"
#include"Util.h"
#include"hydrophones.h"
////////////////////PARAMETERS//////////////////

long int datasize = 100000;
// choose datasize such t = timeperiod of ping + T/2
//time of data you want can be changed by changing datasize length
int flt_freq = 40000;
// filtering frequency
//check line 507 to set another frequency filter if you
//find 2 peaks in the spectrum then set freq1 & freq2
int best_pings = 1;
// set this on the basis of the pings you get after visualising it

int range = 200;
// the average of largest (in time domain) are taken for ping thresholding
//change the value to change the thresholding value (displayed in output too)

// and remember you are getting delay not the angle

/////////////////PAIR_FUN/////////////////////////////////
float Pair::getData(long len = datasize)
{
      if(flushData())
      cout<<"data flushing done..!";
      else
      {
        cout<<"data could not flush";
        return false;
      }

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
      cout<<(clock() - begin_t)/CLOCKS_PER_SEC<<"s"<<endl;
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
          return true;
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

    // h1.debug(1);
    // h2.debug(1);
    return true;
  }
  else
  {
    cout<<"file didnt open"<<endl;
    return false;
  }
}

int Pair::correlation(vector< complex<float> > x1, vector< complex<float> > x2)
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

  std::complex<float>* input3 = c.data();
  std::complex<float>* output3 = new std::complex<float>[c.size()];
  fftplan ifft = fft_create_plan(c.size(),reinterpret_cast<liquid_float_complex*>(input3),reinterpret_cast<liquid_float_complex*>(output3), LIQUID_FFT_BACKWARD, 0);
  fft_execute(ifft);

  vector<float> tc;
  for(int i=0; i<c.size(); i++)
    tc.push_back(abs(*output3++));


  rotate(tc.begin(),tc.begin() + (tc.size()/2),tc.end());
  int late = std::max_element(tc.begin(),tc.end()) - tc.begin() - tc.size()/2;
  return late;

}

int Pair::correlation(vector< complex<float> > x1, vector< complex<float> > x2,ofstream& file)
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

  std::complex<float>* input3 = c.data();
  std::complex<float>* output3 = new std::complex<float>[c.size()];
  fftplan ifft = fft_create_plan(c.size(),reinterpret_cast<liquid_float_complex*>(input3),reinterpret_cast<liquid_float_complex*>(output3), LIQUID_FFT_BACKWARD, 0);
  fft_execute(ifft);

  vector<float> tc;
  for(int i=0; i<c.size(); i++)
    tc.push_back(abs(*output3++));


  rotate(tc.begin(),tc.begin() + (tc.size()/2),tc.end());

  if(file.is_open())
  {
    for(int i=0;i<tc.size();i++)
    file<<tc.at(i)<<endl;

    // cout<<"correlation file done"<<endl;
  }
  else
  cout<<"file couldnot open"<<endl;


  int late = std::max_element(tc.begin(),tc.end()) - tc.begin() - tc.size()/2;
  return late;

}

void Pair::smooth()
{


}

double Pair::delay_modified(double data1[],double data2[], int n, Hydrophones* hgui)
{
  if(flushData())
  cout<<"data flushing done..!";
  else
  {
    return 101;
  }

std::complex<float> temp1;
std::complex<float> temp2;
  // h1.empty(); this function will empty if anyting is there prv
  int time_plot[n];
  for(int i=0;i<n;i++)
  {
    time_plot[i] = i;
    temp1 = data1[i];
    temp2 = data2[i];
    h1.storeData(temp1);
    h2.storeData(temp2);
  }

  double ans;
  ans = this->delay(hgui);

  return ans;

}

double Pair::delay(Hydrophones* hgui)
{

    // h1.debug(1);
    // h2.debug(1);

    // h1.writeFile(1,"../plots/h1t.txt");
    // h2.writeFile(1,"../plots/h2t.txt");

    h1.calFreq();
    h2.calFreq();


    // h1.debug(2);
    // h2.debug(2);

    // h2.writeFile(2,"../plots/h2fr.txt");
    // h1.writeFile(2,"../plots/h1fr.txt");

    h1.filter(Fs);
    h2.filter(Fs);

    h1.debug(1,hgui,1);
    h2.debug(2,hgui,1);

    h1.debug(3,hgui,2);
    h2.debug(4,hgui,2);


    // h2.writeFile(2,"../plots/h2filt.txt");
    // h1.writeFile(2,"../plots/h1filt.txt");

    // h1.writeFile(1,"../plots/h1tfilt.txt");
    // h2.writeFile(1,"../plots/h2tfilt.txt");
    h1.peakFinder();
    h1.debug(4);
    sort(h1.peaks.begin(),h1.peaks.end());

    // h1.writeFile(3,"../plots/h1fall.txt");
    // h1.writeFile(4,"../plots/h1peaks.txt");
    double delay = 0;
    int win = datasize/100;

    vector< vector< complex<float> > > v1;
    // ofstream file1("../plots/h1working.txt");
    // if(file1.is_open())
    // {
      for(int i=0;i<h1.peaks.size();i++)
      {
        // v1.push_back(h1.peakExtraction(h1.peaks[i],win,file1));
        v1.push_back(h1.peakExtraction(h1.peaks[i],win));
      }
    // }
    // else
    // cout<<"file could not open"<<endl;
    // file1.close();

    vector< vector< complex<float> > > v2;
    // ofstream file2("../plots/h2working.txt");
    // if(file2.is_open())
    // {
      for(int i=0;i<h1.peaks.size();i++)
      {
        // v2.push_back(h2.peakExtraction(h1.peaks[i],win,file2));
        v2.push_back(h2.peakExtraction(h1.peaks[i],win)); //note here hyd1 peaks are used
      }
    // }
    // else
    // cout<<"file could not open"<<endl;
    // file2.close();

    cout<<"both peak extraction done"<<endl;

    vector<int> d;
    ofstream file3("../plots/correlation.txt");
    if(file3.is_open())
    {  for(int i=0;i<h1.peaks.size();i++)
      {
        d.push_back(correlation(v1[i],v2[i],file3)); //note here hyd1 peaks are used
        cout<<d.at(i)<<endl;
      }
    }
    else
    cout<<"file could not open"<<endl;
    file3.close();
    cout<<"convolution done ;)"<<endl;
    double ans;
    double std;
    ans = mean(d);
    std = stdDev(d);
    cout<<"calculated delay:"<<ans<<endl;
    cout<<"std deviation:"<<std<<endl;

    return ans;

}
/////////////////////HYD-FUN//////////////////////////
vector< complex<float> >  Hydrophone::peakExtraction(int x, int len)
  {    // cout<<filename<<" ";

      std::vector<float> v;
      complex<float> temp;
      vector< complex<float> > subVec;

      for(int i=0; i<len;i++)
      {
        v.push_back(abs(tdata.at(x-floor(len/2)+i)));
        temp = v.at(i);
        subVec.push_back(temp);
      }
      cout<<"peak extracted for:"<<x<<endl;
      // writeFile(v,file);

  return subVec;
}


vector< complex<float> >  Hydrophone::peakExtraction(int x, int len,ofstream& file)
  {    // cout<<filename<<" ";

      std::vector<float> v;
      complex<float> temp;
      vector< complex<float> > subVec;

      for(int i=0; i<len;i++)
      {
        v.push_back(abs(tdata.at(x-floor(len/2)+i)));
        temp = v.at(i);
        subVec.push_back(temp);
      }
      // cout<<"peak extracted for:"<<x<<endl;
      writeFile(v,file);

  return subVec;
}

void Hydrophone::writeFile(vector<float> v, ofstream& file)
{

  if(file.is_open())
  {
    for(int i=0;i<v.size();i++)
    file<<v.at(i)<<endl;

    // cout<<" file done..!"<<endl;
  }
  else
  cout<<"file couldnot open"<<endl;

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

  cout<<"going for wfall.. XD "<<endl;
  int win;

  vector< pair<int,int> > wfall;

  for(int i=0;i<pkind.size();i++)
  {   float max = 0;
  		if(pkind.at(i) == 1)
  		 {
         //cout<<i<<" ";
          win = 40000;
          float max = 0;
      		// int count = 0;
      		int avg = 0;
          int j =0;
  				for(j = 0; (j<win && (j+i)<pkind.size());j++)
  				{
  					if(pkind.at(i+j) == 1)
  					{
              if(abs(tdata.at(i+j+beginning))>max)
              {
                avg = (i+j+beginning);
                max = abs(tdata.at(i+j+beginning));
              }
              // count++;
  					}

  				}
        i = i+j;
        // cout<<avg<<endl;
  			wfall.push_back(make_pair(max,avg));
  		}
  }

  cout<<"wfall size:"<<wfall.size()<<endl;
  sort(wfall.begin(),wfall.end(),greater< pair<int,int> >());
  // int push_bias = 2500;
  for(int i=0;i<best_pings;i++)
  {
    // cout<<wfall[i].second<<endl;
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
      file<<abs(tdata.at(i))<<endl;

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
    {
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
    cout<<"tdata size in hyd"<<ID<<": "<<tdata.size()<<endl;
  }
  if(i == 2)
  {
    cout<<"frq size in hyd"<<ID<<": "<<fdata.size()<<endl;
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

void Hydrophone::debug(int i,Hydrophones* hgui,int choice)
{

  int time_plot[datasize];
  for(int i=0;i<datasize;i++){time_plot[i]=i;}
  if (choice == 1)
  {

    if(i == 1)
    {
      double temp[datasize];
      convtDouble(tdata,datasize,temp);
      hgui->plotTdata(temp,time_plot,datasize);
    }
    if(i == 2)
    {
      double temp[datasize];
      convtDouble(tdata,datasize,temp);
      hgui->plotFdata(temp,time_plot,datasize);
    }
    if(i == 3)
    {
      double temp[datasize];
      convtDouble(tdata,datasize,temp);
      hgui->plotSegment(temp,time_plot,datasize);
    }
    if(i == 4)
    {
      double temp[datasize];
      convtDouble(tdata,datasize,temp);
      hgui->plotCorrelation(temp,time_plot,datasize);
    }
  }

  if (choice == 2)
  {

    if(i == 1)
    {
      double temp[datasize];
      convtDouble(fdata,datasize,temp);
      hgui->plotTdata(temp,time_plot,datasize);
    }
    if(i == 2)
    {
      double temp[datasize];
      convtDouble(fdata,datasize,temp);
      hgui->plotFdata(temp,time_plot,datasize);
    }
    if(i == 3)
    {
      double temp[datasize];
      convtDouble(fdata,datasize,temp);
      hgui->plotSegment(temp,time_plot,datasize);
    }
    if(i == 4)
    {
      double temp[datasize];
      convtDouble(fdata,datasize,temp);
      hgui->plotCorrelation(temp,time_plot,datasize);
    }
  }

  if (choice == 3)
  {

    if(i == 1)
    {
      double temp[datasize];
      convtDouble(pkind,datasize,temp);
      hgui->plotTdata(temp,time_plot,datasize);
    }
    if(i == 2)
    {
      double temp[datasize];
      convtDouble(pkind,datasize,temp);
      hgui->plotFdata(temp,time_plot,datasize);
    }
    if(i == 3)
    {
      double temp[datasize];
      convtDouble(pkind,datasize,temp);
      hgui->plotSegment(temp,time_plot,datasize);
    }
    if(i == 4)
    {
      double temp[datasize];
      convtDouble(pkind,datasize,temp);
      hgui->plotCorrelation(temp,time_plot,datasize);
    }
  }

}



void Hydrophone::calFreq()
{
  // float complex * y = (float complex*) malloc(tdata.size() * sizeof(float complex));
  // tdata.pop_back();
  std::complex<float>* input = tdata.data();
  std::complex<float>* output = new std::complex<float>[tdata.size()];
  fftplan fft = fft_create_plan(tdata.size(),reinterpret_cast<liquid_float_complex*>(input),reinterpret_cast<liquid_float_complex*>(output), LIQUID_FFT_FORWARD, 0);
  fft_execute(fft);
  complex<float> temp;
  for(int i=0; i<tdata.size(); i++)
  {
  // cout<<abs(*output++)<<" ";
  temp = (*output++);//
  fdata.push_back(temp);
  }
  fft_destroy_plan(fft);
  cout<<"fft done successfully :) for hyd"<<ID<<endl;

}

void Hydrophone::filter(float Fs)
{
  // cout<<Fs<<" "<<fdata.size()<<endl;
  //check the frequency domain of the data and set these peaks
  if(fdata.size() != 0)
  { float b = (float)(Fs)/(float)(fdata.size());
    // cout<<b<<endl;
    // vector< complex<float> > fdata_filt(fdata.size(),0);
    int fpass1 = 78000;//remember the order fpass1<fpass2
    int fpass2 = 82000;//if you are getting any

    int fpass3 = flt_freq - 2000; //remember the order fpass3<fpass4
    int fpass4 = flt_freq + 2000;


    for(int i=0;i<fdata.size();i++)
    {
      float freq = ((float)i*b);
      if((freq>fpass1 && freq<fpass2) ||( (freq>Fs-fpass2) && (freq<Fs-fpass1) ))
      continue;
      else if ((freq>fpass3 && freq<fpass4) ||( (freq>Fs-fpass4) && (freq<Fs-fpass3) ))
      continue;
      else
      fdata.at(i) = 0;
    }


    std::complex<float>* input = fdata.data();
    std::complex<float>* output = new std::complex<float>[fdata.size()];
    fftplan ifft = fft_create_plan(fdata.size(),reinterpret_cast<liquid_float_complex*>(input),reinterpret_cast<liquid_float_complex*>(output), LIQUID_FFT_BACKWARD, 1);
    fft_execute(ifft);

    complex<float> temp;
    for(int i=0; i<fdata.size(); i++)
    {

    temp = abs(*output++);
    tdata.at(i) = temp.real()*(1.0/fdata.size());
    }

    cout<<"filtering Done for hyd"<<ID<<endl;
    fft_destroy_plan(ifft);


  }
  else
  cout<<"frequency domain khali..!";

}

/////////////////////MAIN FUNCTION///////////////////////
int main(int argc, char** argv)
{
 QApplication a(argc, argv);
 Hydrophones h;
 Hydrophones* hgui = &h;

  char* filename = "../pinger_data/l90.txt";
  ifstream file;
  std::complex<float> temp;
  file.open(filename);


  if(file.is_open())
  {
    double Fs;
    file>>Fs;
    Pair p1(Fs);
    cout<<Fs<<endl;
    char choice = 'n';

    while(!file.eof())
    {
      hgui->show();
      float data;
      double delay =0;
      double arr1[datasize];
      double arr2[datasize];

      int begin_ignore = 20000;
      for(int i=0;i<begin_ignore;i++)
      {
        double temp;
        file>>temp;
      }

      for(int i=0;i<datasize;i++)
      {
        file>>data;
        arr1[i] = data;

        file>>data;
        arr2[i] = data;
      }

      // hgui->plotTdata(arr1,time_plot,datasize);

      delay = p1.delay_modified(arr1,arr2,datasize,hgui);
      cout<<delay<<endl;
      if (delay == 101)
      {
        cout<<"Data could not be flushed/Process interupted\n";
      }

      a.exec();
      hgui->resetGraphAll();

      cout<<"enter y to continue:";
      cin>>choice;

      if(choice == 'y')
        continue;
      else
        break;
    }
    cout<<"file closed"<<endl;
    file.close();

  }


return 0;
}
