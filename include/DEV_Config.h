/*****************************************************************************
* | File      	:   DEV_Config.h
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-21
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "Debug.h"

#include <bcm2835.h>
  



/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * GPIOI config
**/
//extern int DEV_RST_PIN;
//extern int DEV_CS_PIN_V;
//extern int DEV_CS_PIN_I;
//extern int DEV_DRDY_PIN;


#define N_SAMPLES 10000
#define N_BUFFERS 2

typedef enum 
{
	BUF_STATE_EMPTY,
	BUF_STATE_FULL,
	BUF_STATE_SENT,

}BUF_STATE;


typedef struct ads
{
	int DEV_CS_PIN;
	int DEV_DRDY_PIN;
	int DEV_RST_PIN;
	int buf_select; // preenchimento sequencial de buffers
	int ads_identifier; // ads 1 or ads 2
	//ADS_DATA* adr1;
	int buf_status[N_BUFFERS]; // empty, full or sent
	double* data[N_BUFFERS]; // data em V ou I
	double* tempo[N_BUFFERS]; //tempo da leitura
} ADS;



/*------------------------------------------------------------------------------------------------------*/
void DEV_Digital_Write(UWORD Pin, UBYTE Value);
UBYTE DEV_Digital_Read(UWORD Pin);

UBYTE DEV_SPI_WriteByte(UBYTE Value);
UBYTE DEV_SPI_ReadByte(void);

UBYTE DEV_Module_Init(ADS placa);
void DEV_Module_Exit(ADS placa);

void DEV_Delay_ms(UDOUBLE xms);
#endif
