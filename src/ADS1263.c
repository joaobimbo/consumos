/*****************************************************************************
* | File      	:   ADS1263.c
* | Author      :   Waveshare team
* | Function    :   ADS1263 driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-10-28
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
#include "ADS1263.h"

UBYTE ScanMode = 0;

/******************************************************************************
function:   Module reset
parameter:
Info:
******************************************************************************/
static void ADS1263_reset(ADS placa)
{
    DEV_Digital_Write(placa.DEV_RST_PIN, 1);
    DEV_Delay_ms(300);
    DEV_Digital_Write(placa.DEV_RST_PIN, 0);
    DEV_Delay_ms(300);
    DEV_Digital_Write(placa.DEV_RST_PIN, 1);
    DEV_Delay_ms(300);
}

/******************************************************************************
function:   send command
parameter: 
        Cmd: command
Info:
******************************************************************************/
static void ADS1263_WriteCmd(UBYTE Cmd,ADS placa)
{
    DEV_Digital_Write(placa.DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(Cmd);
    DEV_Digital_Write(placa.DEV_CS_PIN, 1);

}

/******************************************************************************
function:   Write a data to the destination register
parameter: 
        Reg : Target register
        data: Written data
Info:
******************************************************************************/
static void ADS1263_WriteReg(UBYTE Reg, UBYTE data,ADS placa)
{
    DEV_Digital_Write(placa.DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(CMD_WREG | Reg);
    DEV_SPI_WriteByte(0x00);
    DEV_SPI_WriteByte(data);
    DEV_Digital_Write(placa.DEV_CS_PIN, 1);
}
/******************************************************************************
function:   Read a data from the destination register
parameter: 
        Reg : Target register
Info:
    Return the read data
******************************************************************************/
static UBYTE ADS1263_Read_data(UBYTE Reg,ADS placa)
{
    UBYTE temp = 0;
    DEV_Digital_Write(placa.DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(CMD_RREG | Reg);
    DEV_SPI_WriteByte(0x00);
    // DEV_Delay_ms(1);
    temp = DEV_SPI_ReadByte();
    DEV_Digital_Write(placa.DEV_CS_PIN, 1);
    return temp;
}


UDOUBLE ADS_read_calib_reg(ADS placa)
{
    UDOUBLE read = 0;
    UBYTE buf[3] = {0, 0, 0};
    DEV_Digital_Write(placa.DEV_CS_PIN, 0);
	
    buf[0] = ADS1263_Read_data(REG_OFCAL0,placa);
    buf[1] = ADS1263_Read_data(REG_OFCAL1,placa);
    buf[2] = ADS1263_Read_data(REG_OFCAL2,placa);
    
    DEV_Digital_Write(placa.DEV_CS_PIN, 1);
    read |= ((UDOUBLE)buf[0] << 16);
    read |= ((UDOUBLE)buf[1] << 8);
    read |= (UDOUBLE)buf[2];

    printf("reg: %x %x %x %x\r\n", buf[0], buf[1], buf[2], read);
    return read;
}
/******************************************************************************
function:   Check data
parameter: 
        val : 4 bytes(ADC2 is 3 bytes) data
		byt : CRC byte
Info:
		Check success, return 0
******************************************************************************/
static UBYTE ADS1263_Checksum(UDOUBLE val, UBYTE byt)
{
	UBYTE sum = 0;
	UBYTE mask = -1;		// 8 bits mask, 0xff
	while(val) {
		sum += val & mask;	// only add the lower values
		val >>= 8;			// shift down
	}
	sum += 0x9b;
	// printf("--- %x %x--- \r\n", sum, byt);
	return sum ^ byt;		// if equal, this will be 0
}

/******************************************************************************
function:   Waiting for a busy end
parameter: 
Info:
    Timeout indicates that the operation is not working properly.
******************************************************************************/
static void ADS1263_WaitDRDY(ADS placa)
{
	// printf("ADS1263_WaitDRDY \r\n");
    UDOUBLE i = 0;
    while(1) {
		if(DEV_Digital_Read(placa.DEV_DRDY_PIN) == 0)
			break;
		if(i >= 4000000) {
			printf("Time Out ...\r\n"); 
			break;
		}
    }	
	// printf("ADS1263_WaitDRDY Release \r\n");
}

/******************************************************************************
function:  Read device ID
parameter: 
Info:
******************************************************************************/
UBYTE ADS1263_ReadChipID(ADS placa)
{
    UBYTE id;
    id = ADS1263_Read_data(REG_ID,placa);
	printf("ID: %d\n",id);
    return id>>5;
}

/******************************************************************************
function:  Setting mode
parameter: 
    Mode : 0 Single-ended input
           1 channel1 Differential input
Info:
******************************************************************************/
void ADS1263_SetMode(UBYTE Mode)
{
    if(Mode == 0) {
        ScanMode = 0;
    }else {
        ScanMode = 1;
    }
}

/******************************************************************************
function:  Configure ADC gain and sampling speed
parameter: 
    gain : Enumeration type gain
    drate: Enumeration type sampling speed
Info:
******************************************************************************/
void ADS1263_ConfigADC1(ADS1263_GAIN gain, ADS1263_DRATE drate, ADS1263_DELAY delay,ADS placa)
{
	UBYTE MODE2 = 0x80;				//0x80:PGA bypassed, 0x00:PGA enabled
	MODE2 |= (gain << 4) | drate;
	ADS1263_WriteReg(REG_MODE2, MODE2,placa);
    DEV_Delay_ms(1);
	if(ADS1263_Read_data(REG_MODE2,placa) == MODE2)
		printf("REG_MODE2 success \r\n");
	else
		printf("REG_MODE2 unsuccess \r\n");
	
	UBYTE REFMUX = 0x00;		//0x00:+-2.5V as REF, 0x24:VDD,VSS as REF
	ADS1263_WriteReg(REG_REFMUX, REFMUX, placa);
	DEV_Delay_ms(1);
	if(ADS1263_Read_data(REG_REFMUX,placa) == REFMUX)
		printf("REG_REFMUX success \r\n");
	else
		printf("REG_REFMUX unsuccess \r\n");
	
	UBYTE MODE0 = delay;
	ADS1263_WriteReg(REG_MODE0, MODE0, placa);
	DEV_Delay_ms(1);
	if(ADS1263_Read_data(REG_MODE0,placa) == MODE0)
		printf("REG_MODE0 success \r\n");
	else
		printf("REG_MODE0 unsuccess \r\n");
}



/******************************************************************************
function:  Device initialization
parameter: 
Info:
******************************************************************************/
UBYTE ADS1263_init_ADC1(ADS1263_DRATE rate,ADS placa)
{
    ADS1263_reset(placa);
    if(ADS1263_ReadChipID(placa) == 1) {
        printf("ID Read success \r\n");
    }
    else {
        printf("ID Read failed \r\n");
        return 1;
    }
	ADS1263_WriteCmd(CMD_STOP1,placa);
	ADS1263_ConfigADC1(ADS1263_GAIN_1, rate, ADS1263_DELAY_35us,placa);
	ADS1263_WriteCmd(CMD_START1,placa);
    return 0;
}


/******************************************************************************
function:  Set the channel to be read
parameter: 
    Channal : Set channel number
Info:
******************************************************************************/
static void ADS1263_SetChannel(UBYTE Channal,ADS placa)
{
    if(Channal > 10) {
        return ;
    }
	UBYTE INPMUX = (Channal << 4) | 0x0a;		//0x0a:VCOM as Negative Input
    ADS1263_WriteReg(REG_INPMUX, INPMUX, placa);
	if(ADS1263_Read_data(REG_INPMUX, placa) == INPMUX) {
		// printf("ADS1263_ADC1_SetChannal success \r\n");
	} else {
		printf("ADS1263_ADC1_SetChannel unsuccess \r\n");
	}
}


/******************************************************************************
function:  Set the channel to be read
parameter: 
    Channal : Set channel number
Info:
******************************************************************************/
void ADS1263_SetDiffChannel(UBYTE Channel,ADS placa) //not needed?
{
	UBYTE INPMUX;
    if (Channel == 0) {
		INPMUX = (0<<4) | 1;	//DiffChannel	AIN0-AIN1
    } else if(Channel == 1) {
		INPMUX = (2<<4) | 3;	//DiffChannal	AIN2-AIN3
    } else if(Channel == 2) {
		INPMUX = (4<<4) | 5;	//DiffChannal	AIN4-AIN5
    } else if(Channel == 3) {
		INPMUX = (6<<4) | 7;	//DiffChannal	AIN6-AIN7
    } else if(Channel == 4) {
		INPMUX = (8<<4) | 9;	//DiffChannal	AIN8-AIN9
    }
	ADS1263_WriteReg(REG_INPMUX, INPMUX,placa); 	
	if(ADS1263_Read_data(REG_INPMUX,placa) == INPMUX) {
		// printf("ADS1263_SetDiffChannal success \r\n");
	} else {
		printf("ADS1263_SetDiffChannal unsuccess \r\n");
	}
}



void calibrate(ADS placa){
  
    /*ADS1263_WriteReg(REG_INPMUX, 0xff,placa); 	
    
    
       
    DEV_Digital_Write(placa.DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(CMD_START1);
    DEV_Digital_Write(placa.DEV_CS_PIN, 1);

       
    DEV_Digital_Write(placa.DEV_CS_PIN, 0);
    DEV_SPI_WriteByte(CMD_SFOCAL1);
    DEV_Digital_Write(placa.DEV_CS_PIN, 1);

    ADS1263_WaitDRDY(placa); 
*/
    //UDOUBLE read = ADS_read_calib_reg(placa);
    UBYTE OFFSET=0x00; //0x6f;
    ADS1263_WriteReg(REG_OFCAL2, OFFSET,placa);
    if(ADS1263_Read_data(REG_OFCAL2,placa) == OFFSET) {
		 printf("offset success \r\n");
	} else {
		printf("offset unsuccess \r\n");
	} 	

    
}
/******************************************************************************
function:  Read ADC data
parameter: 
Info:
******************************************************************************/
static UDOUBLE ADS1263_Read_ADC1_Data(ADS placa)
{
    UDOUBLE read = 0;
    UBYTE buf[4] = {0, 0, 0, 0};
	UBYTE Status, CRC;
    DEV_Digital_Write(placa.DEV_CS_PIN, 0);
	do {
		DEV_SPI_WriteByte(CMD_RDATA1);
		Status = DEV_SPI_ReadByte();
        //if(Status!=0x69)        
        //printf("STATUS: %x\n",Status);
	}while((Status & 0x40) == 0);
	 DEV_Delay_ms(0);

    buf[0] = DEV_SPI_ReadByte();
    buf[1] = DEV_SPI_ReadByte();
    buf[2] = DEV_SPI_ReadByte();
    buf[3] = DEV_SPI_ReadByte();
	CRC = DEV_SPI_ReadByte();
    DEV_Digital_Write(placa.DEV_CS_PIN, 1);
    read |= ((UDOUBLE)buf[0] << 24);
    read |= ((UDOUBLE)buf[1] << 16);
    read |= ((UDOUBLE)buf[2] << 8);
	read |= (UDOUBLE)buf[3];
    // printf("%x %x %x %x %x %x\r\n", Status, buf[0], buf[1], buf[2], buf[3], CRC);
	if(ADS1263_Checksum(read, CRC) != 0)
		printf("ADC1 Data read error! \r\n");
    return read;
}



/******************************************************************************
function:  Read ADC specified channel data
parameter: 
    Channel: Channel number
Info:
******************************************************************************/
UDOUBLE ADS1263_GetChannelValue(UBYTE Channel,ADS placa) 
{
    UDOUBLE Value = 0;
    if(ScanMode == 0) {// 0  Single-ended input  10 channel1 Differential input  5 channe 
        if(Channel>10) {
            return 0;
        }
        ADS1263_SetChannel(Channel,placa);
		// DEV_Delay_ms(2);
        // ADS1263_WriteCmd(CMD_START1);
        // DEV_Delay_ms(2);
		ADS1263_WaitDRDY(placa);
        Value = ADS1263_Read_ADC1_Data(placa);
    } else {
        if(Channel>4) {
            return 0;
        }
        //ADS1263_SetDiffChannel(Channel,placa);
		// DEV_Delay_ms(2);
        // ADS1263_WriteCmd(CMD_START1);
        // DEV_Delay_ms(2);
		//ADS1263_WaitDRDY(placa);
        Value = ADS1263_Read_ADC1_Data(placa);
    }
	// printf("Get IN%d value success \r\n", Channel);
    return Value;
}


