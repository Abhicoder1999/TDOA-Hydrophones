/*	gcc adc.c -lbcm2835       */
#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <fstream>
#define ADC_CONVST_2 RPI_BPLUS_GPIO_J8_31
#define ADC_CONVST_1 RPI_BPLUS_GPIO_J8_35 
#define ADC_DIN_1 RPI_BPLUS_GPIO_J8_37
#define ADC_DIN_2 RPI_BPLUS_GPIO_J8_33

using namespace std;
int main(int argc, char* argv[]){
	if (!bcm2835_init()){  printf("bcm2835_init failed.\n");
        	exit(0);}
    	if (!bcm2835_spi_begin()){ printf("bcm2835_spi_begin failed.\n");
		exit(0);}
    	//bcm2835_gpio_fsel(RPI_BPLUS_GPIO_J8_38, BCM2835_GPIO_FSEL_OUTP);
    	//bcm2835_gpio_fsel(RPI_BPLUS_GPIO_J8_40, BCM2835_GPIO_FSEL_OUTP);
    	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      
    	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);  
    	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_8);
    	//bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);         
    	bcm2835_gpio_fsel(ADC_DIN_1, BCM2835_GPIO_FSEL_OUTP);
    	bcm2835_gpio_fsel(ADC_DIN_2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(ADC_CONVST_1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(ADC_CONVST_2, BCM2835_GPIO_FSEL_OUTP);
  	//bcm2835_gpio_write(RPI_BPLUS_GPIO_J8_40, HIGH);
  	//bcm2835_gpio_write(RPI_BPLUS_GPIO_J8_38, LOW);

	long 	adcbinarydata;
	float 	adcvoltage;
	int len = 1000000;
    uint8_t read_data[len];
	uint8_t read_data1[len];
	uint8_t read_data2[len];

	uint8_t read[len];
	uint8_t read1[len];
	uint8_t read2[len];
	
	clock_t begin_t;
	clock_t clk_check1;
	ofstream test;
	test.open(argv[1]);
    cout<<"done0\n";
        
    	bcm2835_gpio_write(ADC_DIN_1, LOW);
    	bcm2835_gpio_write(ADC_DIN_2, LOW);
    	bcm2835_gpio_write(ADC_CONVST_1, LOW);
		bcm2835_gpio_write(ADC_CONVST_2, LOW);
	
    	begin_t = clock();
    	for(int i=0; i<len; i++){

    		bcm2835_gpio_write(ADC_DIN_1, HIGH);
			bcm2835_gpio_write(ADC_DIN_2, HIGH);
    		bcm2835_gpio_write(ADC_CONVST_1, HIGH);
			bcm2835_gpio_write(ADC_CONVST_2, HIGH);
//  		bcm2835_spi_transfer(0);
			bcm2835_delayMicroseconds(1);
			
    		bcm2835_gpio_write(ADC_DIN_1, LOW);
    	read[i]  = bcm2835_spi_transfer(0);
		read1[i] = bcm2835_spi_transfer(0);
		read2[i] = bcm2835_spi_transfer(0);
			
	
   		bcm2835_gpio_write(ADC_DIN_2, LOW);
		read_data[i]  = bcm2835_spi_transfer(0);
		read_data1[i] = bcm2835_spi_transfer(0);
		read_data2[i] = bcm2835_spi_transfer(0);
		
    	bcm2835_gpio_write(ADC_CONVST_1, LOW);
		bcm2835_gpio_write(ADC_CONVST_2, LOW);

	}
	cout << (CLOCKS_PER_SEC*(float)len/(clock() - begin_t)) << "Hz" << endl;
	test<<CLOCKS_PER_SEC*(float)len/(clock() - begin_t)<<endl;
	for(int i=0; i<len; i++)
	{	
		read_data[i] = read_data[i] & 0x7F;
		adcbinarydata = (read_data[i] << 11) + (read_data1[i] << 3) + (read_data2[i] >> 5);
		test<<adcbinarydata<<endl;
		
		read[i] = read[i] & 0x7F;
		adcbinarydata = (read[i] << 11) + (read1[i] << 3) + (read2[i] >> 5);
		test<<adcbinarydata<<endl;
	}
    	bcm2835_spi_end();
    	bcm2835_close();
return 0;
}
