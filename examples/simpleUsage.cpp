/*
#	-------------------------------------------------------
#	RASPBERRY PI AHT21 SIMPLE USAGE EXAMPLE
#	
#	Simple example code for reading AHT21 sensor data 
#
#	build with:
#		g++ -Wall simpleUsage.cpp -o example -lpigpio
#	-------------------------------------------------------
*/

#include <iostream>
#include <pigpio.h>

#include "aht21.cpp"

int main()
{
	// initialize the pigpio library
	gpioInitialise();

	// initialize the AHT21 sensor
	AHT21_init();

	// there are two methods of using this lib
	// 1: use the get-functions:

	std::cout << "Humidity: " << AHT21_getHum() << "%" << std::endl;
	std::cout << "Temperature: " << AHT21_getTemp() << "°C" << std::endl;;

	// 2: use the variables directly :
	// (faster beacause it reads the sensor data only once)

	AHT21_read();
	std::cout << "Humidity: " << AHT21_humid << "%" << std::endl;
	std::cout << "Temperature: " << AHT21_tempC << "°C" << std::endl;;

	//close pigpio
    gpioTerminate();

    
    return 0;
}