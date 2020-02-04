#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_spi.h"

void SPIAcc_Init(void){
            SPI_InitTypeDef SPI_InitTypeDefStruct;
            GPIO_InitTypeDef GPIO_InitTypeDefStruct;
            // Enable clock to SPI1
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
            // Setup SPI to be master
            SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
            SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
            SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;
            SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_Low;
            SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_1Edge;
            SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
            SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
            SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
            SPI_Init(SPI1, &SPI_InitTypeDefStruct);
            // Enable clocks to Ports A and E
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE , ENABLE);
            // Set SPI pins on PortA (A5,A6,A7) to Alternate Function Mode
            GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6;
            GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
            GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
            GPIO_Init(GPIOA, &GPIO_InitTypeDefStruct);
            // Pin E3 is SPI_CS (chip select) 
            GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_3;
            GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
            GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
            GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
            GPIO_Init(GPIOE, &GPIO_InitTypeDefStruct);
            // Connect the PortA pins to the alternate function 
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
            // Hold CS high 
            GPIO_SetBits(GPIOE, GPIO_Pin_3);
            // Enable the SPI module
            SPI_Cmd(SPI1, ENABLE);
}
 

void SPIAcc_SendByte(uint8_t address, uint8_t data){
  
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
      
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
    SPI_I2S_SendData(SPI1, address);
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    SPI_I2S_ReceiveData(SPI1);
      
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
    SPI_I2S_SendData(SPI1, data);
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    SPI_I2S_ReceiveData(SPI1);
      
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
}
 

uint8_t SPIAcc_GetByte(uint8_t address){
  
    GPIO_ResetBits(GPIOE, GPIO_Pin_3); 
      
    address = 0x80 | address;
      
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
    SPI_I2S_SendData(SPI1, address);
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    SPI_I2S_ReceiveData(SPI1); //Clear RXNE bit
      
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
    SPI_I2S_SendData(SPI1, 0x00); //Dummy byte to generate clock
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
      
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
      
    return  SPI_I2S_ReceiveData(SPI1);
}