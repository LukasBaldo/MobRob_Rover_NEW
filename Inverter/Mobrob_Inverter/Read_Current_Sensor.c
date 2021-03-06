//Read Current  via SPI-Interface
#include "Read_Current_Sensor.h"

float readCurrent(uint8_t ChipSelect){
	uint8_t ReadData[2];

	BUS_IO_Write(&SPI_ChipSelect, ChipSelect);
	SPI_MASTER_Receive(&SPI_MASTER_0, ReadData, 2U);
	BUS_IO_Write(&SPI_ChipSelect, 0b111);
	ReadData[0] = ReadData[0] & 0X1F; 									//cutting away the 3 info bits
	uint16_t iCurrent = ReadData[0]<<8 | ReadData[1];					//combine Data

	// calculation of the current
	float fCurrent = ((float)iCurrent - 4096.0)/160.0;					// ATTENTION: value has to be divided by 160 for the 25A Sensor and 80 for the 50A Sensor

	return fCurrent;
}
