#include <unistd.h>
#include <stdio.h>

#define AHT21_DEBUG     0           // 1 = display debug messages
#define AHT21_I2CBUS    1           // I2C Bus...       0 = SDA: Pin 0    SCL: Pin 1  
                                    //                  1 = SDA: Pin 2    SCL: Pin 3
int AHT21_ADDRESS = 0x38;

int AHT21_i2cHandle;

float AHT21_tempC = 0;
float AHT21_humid = 0;


int AHT21_cal()
{
	// calibrate command
	char cal_cmd[3] = {0xE1, 0x08, 0x00};
	char ans;
	
	i2cWriteDevice(AHT21_i2cHandle, cal_cmd, 3);
	usleep(500000);
	i2cReadDevice(AHT21_i2cHandle, &ans, 1);
	if( (ans & 0x68) != 0x08)
	{
        //ERROR cheksum missmatch
        if(AHT21_DEBUG == 1)
        {
            std::cout << "AHT21: Error while calibrating" << std::endl;
        }
		return -1;
	}
    else 
    {
        if(AHT21_DEBUG == 1)
        {
            std::cout << "AHT21: Calibration ok" << std::endl;
        }
        return 1;
    }
}

int AHT21_init(int i2cbus = AHT21_I2CBUS, int addr = AHT21_ADDRESS)
{
    AHT21_i2cHandle = i2cOpen(i2cbus, addr, 0);
    if(AHT21_DEBUG == 1)
    {
        if(AHT21_i2cHandle < 0)
        {
            std::cout << "AHT21: Error while creating i2handle. Code: " << std::to_string(AHT21_i2cHandle) << std::endl;
        }
    }
    if(AHT21_cal() < 0)
    {
        return -1;
    }
    return AHT21_i2cHandle;
}

void AHT21_read(int handle = AHT21_i2cHandle)
{
	char read_cmd[3] = {0xAC, 0x33, 0x00};
	char temp[6];
	char *tmp = temp;
	
    i2cWriteDevice(handle, read_cmd, 3);
	usleep(100000);
	i2cReadDevice(handle, tmp, 6);

	float calc_hum = ((temp[1] << 16) | (temp[2] << 8) | temp[3]) >> 4;
	float calc_temp = ((temp[3] & 0x0F) << 16) | (temp[4] << 8) | temp[5];
    
    AHT21_humid = calc_hum * 100 / 1048576;
    AHT21_tempC = calc_temp * 200 / 1048576 - 50;
    
    if(AHT21_DEBUG == 1)
    {
	    std::cout << "Humidity: " << AHT21_humid << " %"<< std::endl;
        std::cout << "Temperature: " << AHT21_tempC << " °C" << std::endl;
    }
}

float AHT21_getTemp()
{
    AHT21_read();
    return AHT21_tempC;
}
float AHT21_getHum()
{
    AHT21_read();
    return AHT21_humid;
}