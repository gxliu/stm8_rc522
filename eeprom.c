#include"iostm8s103f3.h"
#include"eeprom.h"
#include"timer2.h"

#if USE_EEPROM == INTERNAL

extern char timeout2;

void eeprom_lock(void)
{
	FLASH_IAPSR&=~0X08;//Clear DUL bit
}

void eeprom_unlock(void)
{
	//FLASH_CR1|=0X02;//IE
	//FLASH_CR2=0XFF;
	if(!(FLASH_IAPSR & 0x08));//DUL bit is set?
	do{
		FLASH_DUKR = 0xAE;// 写入第一个密钥
		FLASH_DUKR = 0x56;// 写入第二个密钥
	}
	while(!(FLASH_IAPSR&0x08));
}

void eeprom_write_byte(char *dst,char dat)
{
	//eeprom_unlock();
	*dst= dat;
	//set_timer2_wait_ms(10);
	while((FLASH_IAPSR & 0x40) == 0 && timeout2!=0);// 等待写操作完成
	//eeprom_lock();
}

char eeprom_read_byte(char *src)
{
	//eeprom_unlock();
	return *src;
	//eeprom_lock();
}

void eeprom_write_block(char *dst,char *src,unsigned char cnt)
{
	//eeprom_unlock();
	for(unsigned char i=0; i<cnt ;i++)
	{
		*(dst+i) = *(src+i);
		//set_timer2_wait_ms(10);
		while((FLASH_IAPSR & 0x40) == 0/* && timeout2!=0*/);// 等待写操作完成
	}
	//eeprom_lock();
}

void eeprom_read_block(char *dst,char *src,unsigned char cnt)//n<=16连续读不能超过16
{
	//eeprom_unlock();
	for(unsigned char i=0; i<cnt ;i++)
	{
		*(dst+i) = *(src+i);
	}
	//eeprom_lock();
}

/*void rop_enable()
{
eeprom_unlock();
FLASH_CR2 |= 0x80;
FLASH_NCR2&=~0x80;
char *rop;
rop=(char*)0x4800;//readout protection
*rop=0xAA;
while(!(FLASH_IAPSR&0x04));
FLASH_CR2 &=~0x80;
FLASH_NCR2|= 0x80;
}

void option_byte_write(char *addr, char dat)
{
eeprom_unlock();
FLASH_CR2 |= 0x80;
FLASH_NCR2&=~0x80;
*addr=dat;
*(addr+1)=~dat;
while(!(FLASH_IAPSR&0x04));
FLASH_CR2 &=~0x80;
FLASH_NCR2|= 0x80;
}*/

#pragma vector=FLASH_EOP_vector
__interrupt void FLASH_EOP_ISR()
{
}

#else
#include"at24c04.h"
/*从指定地址读一个字节出来*/
char eeprom_read_byte(short src)
{
	char data;
	char page=(char)((src&0x300)>>7);
	//	 I2C_Ini();
	I2C_Start();
	I2C_Write(WRITE|page);
	I2C_Write(src&0xff);
	I2C_Start();
	I2C_Write(READ|page);
	data=I2C_Read();
	I2C_NAK();
	I2C_Stop();
	return data;
}

/*从一个地址开始连续读出字符串(n个字符)*/
void eeprom_read_block(char *dst,short src,char n)//n<=16，一次连续读写仅限于同一个Page，不能超过16字节
{
	char i,j=0,rem;
	do{
		char page=(char)((src&0x300)>>7);
		//	 I2C_Ini();
		I2C_Start();
		I2C_Write(WRITE|page);
		I2C_Write(src&0xff);
		I2C_Start();
		I2C_Write(READ|page);
		rem=16-src%16;
		rem=((n<rem)?n:rem);
		for(i=1;i<rem;i++)
		{
			dst[j++]=I2C_Read();
			I2C_ACK();
		}
		dst[j++]=I2C_Read();
		I2C_NAK();
		I2C_Stop();
		src+=rem;
		n-=rem;
		timer2_wait_ms(5);
	}while(n);
}

/*向指定地址写一个字节*/
void eeprom_write_byte(short dst, char src)
{
	char page=(char)((dst&0x300)>>7);
	//	 I2C_Ini();
	I2C_Start();
	I2C_Write(WRITE|page);
	I2C_Write(dst&0xff);
	I2C_Write(src);
	I2C_Stop();
}

/*从一个地址开始连续写入字符串*/
void eeprom_write_block(short dst,char *src,char n)//n<=16，一次连续读写仅限于同一个Page，不能超过16字节
{
	char i,j=0,rem;
	do{
		char page=(char)((dst&0x300)>>7);
		I2C_Start();
		I2C_Write(WRITE|page);
		I2C_Write(dst&0xff);
		rem=16-dst%16;
		rem=((n<rem)?n:rem);
		for(i=0;i<rem;i++)
			I2C_Write(src[j++]);
		I2C_Stop();
		dst+=rem;
		n-=rem;
		timer2_wait_ms(5);
	}while(n);
}

#endif
