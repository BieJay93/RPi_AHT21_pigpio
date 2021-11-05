#include <iostream>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <pigpio.h>
#include <vector>
#include "dht22.cpp"
#include "aht21.cpp"


void debug(std::string msg)
{
    std::cout << msg << std::endl;
}

void* threadDHT(void* pin)
{
	while(1)
	{
		DHT22_read(*static_cast<int*>(pin));
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	std::cout << "Thread ended" << std::endl;
}

int main()
{
    debug("Starte Programm");
	gpioInitialise();

	AHT21_init();

	gpioSetPullUpDown(4, PI_PUD_UP);
	int dhtpin = 4;

	AHT21_read();

	pthread_t* thread1 = gpioStartThread(threadDHT, &dhtpin);

    std::string cinStr;
	while (true)
	{
		std::getline(std::cin, cinStr);
		if(cinStr == "q")
		{
			break;
		}
        else if(cinStr == "dht22")
		{
			DHT22_read(4);
		}
		else if(cinStr == "aht21")
		{
			AHT21_read();
		}
		else 
		{
			//testgpio(4, std::stoi(cinStr));
		}
	}
	gpioStopThread(thread1);
    gpioTerminate();

    
    return 0;
}