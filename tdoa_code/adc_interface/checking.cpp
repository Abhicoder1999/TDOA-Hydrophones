/*	gcc adc.c -lbcm2835       */
#include <stdio.h>
#include <bcm2835.h>
#include <stdlib.h>
#define ADC_CONVST RPI_BPLUS_GPIO_J8_35
#define ADC_DIN RPI_BPLUS_GPIO_J8_37

int main(){
	if (!bcm2835_init()){  printf("bcm2835_init failed.\n");
        	exit(0);}
    	if (!bcm2835_spi_begin()){ printf("bcm2835_spi_begin failed.\n");
		exit(0);}
    	//bcm2835_gpio_fsel(RPI_BPLUS_GPIO_J8_38, BCM2835_GPIO_FSEL_OUTP);
    	//bcm2835_gpio_fsel(RPI_BPLUS_GPIO_J8_40, BCM2835_GPIO_FSEL_OUTP);
    	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      
    	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);  
    	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);
    	//bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);         
    	bcm2835_gpio_fsel(ADC_DIN, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(ADC_CONVST, BCM2835_GPIO_FSEL_OUTP);
  	//bcm2835_gpio_write(RPI_BPLUS_GPIO_J8_40, HIGH);
  	//bcm2835_gpio_write(RPI_BPLUS_GPIO_J8_38, LOW);
	uint8_t read_data;
	uint8_t read_data1;
	uint8_t read_data2;
	long 	adcbinarydata;
	float 	adcvoltage;
        
    	bcm2835_gpio_write(ADC_DIN, LOW);
    	bcm2835_gpio_write(ADC_CONVST, LOW);
	double MAX_ADC_VAL = (1 << 18) - 1;
    	while(1){

    		bcm2835_gpio_write(ADC_DIN, HIGH);
//    		bcm2835_delayMicroseconds(1);
    		bcm2835_gpio_write(ADC_CONVST, HIGH);
    		bcm2835_spi_transfer(0);
    		bcm2835_delayMicroseconds(10);
    		bcm2835_gpio_write(ADC_DIN, LOW);
		//m.lock();
    		read_data  = bcm2835_spi_transfer(0);
		read_data1 = bcm2835_spi_transfer(0);
		read_data2 = bcm2835_spi_transfer(0);
		//read_data2 = read_data2&0xE0;
		read_data = read_data & 0x7F;
		adcbinarydata = (read_data << 11) + (read_data1 << 3) + (read_data2 >> 5);
		
		printf("%ld \n",adcbinarydata);
		adcvoltage = (double)adcbinarydata*4.77/MAX_ADC_VAL;

		//char* readData[4] = {0,0,0};
		//bcm2835_spi_transfern(readData, 3);
    		
		//printf("%d, %d , %d\n",read_data,read_data2,read_data2);
		printf("%f \n",adcvoltage);
		
    	//	bcm2835_gpio_write(ADC_DIN, HIGH);
    	//      bcm2835_delayMicroseconds(100);
    		bcm2835_gpio_write(ADC_CONVST, LOW);
    		bcm2835_spi_transfer(0);
    		bcm2835_delayMicroseconds(1000000);
	}
    	bcm2835_spi_end();
    	bcm2835_close();
return 0;
}

