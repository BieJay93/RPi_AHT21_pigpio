/*
#	-------------------------------------------------------
#	RASPBERRY PI AHT21 THREADED EXAMPLE
#	
#	Example code for reading AHT21 sensor data from a subthread
#
#	build with:
#		g++ -Wall threadedUsage.cpp -o example -lpigpio - lpthread
#	-------------------------------------------------------
*/


#include <iostream>
#include <thread>
#include <pigpio.h>

#include "aht21.cpp"

uint8_t running = 1;

void* threadAHT(void* handle)
{
	// read the sensor data every 5 seconds
	while(running)
	{
		//AHT21_read(*static_cast<int*>(handle));
		AHT21_read();
		std::cout << "Humidity: " << AHT21_humid << "%" << std::endl;
		std::cout << "Temperature: " << AHT21_tempC << "°C" << std::endl;;
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	std::cout << "Thread ended" << std::endl;
}


int main()
{
	// initialize the pigpio library
	gpioInitialise();

	AHT21_init();

	// create a new thread and start the sensor reading in background
	pthread_t* thread1 = gpioStartThread(threadAHT, &AHT21_i2cHandle);

	// run until user quits
	std::string cinStr;
	while (true)
	{
		std::getline(std::cin, cinStr);
		if(cinStr == "q")
		{
			running = 0;
			break;
		}
	}

	// stop the subthread
	gpioStopThread(thread1);

	//close pigpio
    gpioTerminate();

    return 0;
}