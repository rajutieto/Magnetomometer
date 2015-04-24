#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

void SPI1_Init(void);							//��ʼ��SPI1��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);	//����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);				//SPI1���߶�дһ���ֽ�

void SPI2_Init(void);							//��ʼ��SPI2��
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);	//����SPI2�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);				//SPI2���߶�дһ���ֽ�

void SPI3_Init(void);							//��ʼ��SPI3��
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);	//����SPI3�ٶ�   
u8 SPI3_ReadWriteByte(u8 TxData);				//SPI3���߶�дһ���ֽ�
		 
#endif