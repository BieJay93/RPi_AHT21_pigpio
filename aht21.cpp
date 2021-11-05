#include <unistd.h>
#include <stdio.h>

#define AHT21_DEBUG     0           // 1 = display debug messages
#define AHT21_I2CBUS    1           // I2C Bus...       0 = SDA: Pin 0    SCL: Pin 1  
                                    //                  1 = SDA: Pin 2    SCL: Pin 3
#define AHT21_ADDRESS   0x38

int i2cHandle;


int AHT21_cal()
{
	// calibrate command
	char cal_cmd[3] = {0xE1, 0x08, 0x00};
	char ans;
	//i2cWriteBlockData(i2cHandle, 0, &cal_cmd,3);
	i2cWriteDevice(i2cHandle, cal_cmd, 3);
	usleep(500000);
	i2cReadDevice(i2cHandle, &ans, 1);
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

int AHT21_init()
{
    i2cHandle = i2cOpen(AHT21_I2CBUS, AHT21_ADDRESS, 0);
    if(AHT21_DEBUG == 1)
    {
        if(i2cHandle >= 0)
        {
            std::cout << "AHT21: Error while creating i2handle. Code: " << std::to_string(i2cHandle) << std::endl;
        }
    }
    if(AHT21_cal() < 0)
    {
        return -1;
    }
    return i2cHandle;
}



void AHT21_read()
{
	char read_cmd[3] = {0xAC, 0x33, 0x00};
	char temp[6];
	char *tmp = temp;
	i2cWriteDevice(i2cHandle, read_cmd, 3);
	usleep(100000);
	i2cReadDevice(i2cHandle, tmp, 6);
	float calc_hum = ((temp[1] << 16) | (temp[2] << 8) | temp[3]) >> 4;
	float calc_temp = ((temp[3] & 0x0F) << 16) | (temp[4] << 8) | temp[5];
    
    if(AHT21_DEBUG == 1)
    {
        std::cout << calc_temp << " & " << calc_hum << std::endl;
    }
	
	std::cout << "Temperatur: " << (((200 * calc_temp) / 1048576) - 50) << " °C" << std::endl;
	std::cout << "Luftfeuchtigkeit: " << (calc_hum * 100 / 1048576) << " %"<< std::endl;
}