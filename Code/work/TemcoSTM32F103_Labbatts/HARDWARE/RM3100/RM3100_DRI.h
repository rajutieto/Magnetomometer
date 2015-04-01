#ifndef _RM3100_DRI_H_
#define _RM3100_DRI_H_
#include "bitmap.h"
 
#define PNI_SENSOR0					0
#define PNI_SENSOR1					1
 
#define TOTAL_AXIS					3
#define X_AXIS						0
#define Y_AXIS						1
#define Z_AXIS						2
 

#define CMM                  0x01 
#define CCX                  0X04
#define CCY                  0X06
#define CCZ                  0X08
#define TMRC                 0X0B
#define MX                   0X24
#define MY                   0X27
#define MZ                   0X2A


#define SPI_STATE0            PEin(3) 
#define SPI_STATE1            PEin(5) 


void PNI_SPI_Init(void);
void spi_write8(u8 address,u8 data_out,u8 sensor);
void spi_write16(u8 address,u16 data_out,u8 sensor);
u8 spi_read8(u8 address,u8 sensor);
u16 spi_read16(u8 address,u8 sensor);
u32 spi_read24(u8 address,u8 sensor);
 
 
 

 
#endif

