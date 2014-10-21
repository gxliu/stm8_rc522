#include "iostm8s103f2.h"
#include "rc522.h"
#include "uart.h"

unsigned char UID[5],Temp[4]                                       ;
unsigned char RF_Buffer[18]                                        ;
unsigned char Password_Buffer[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}   ; // Mifare One 缺省密码

char cmd;
unsigned char des_on       = 0                                     ; // DES加密

void auto_reader(void)
{
	while(1)
	{
		if(PcdRequest(0x52,Temp)==MI_OK)
		{
			if(Temp[0]==0x04&&Temp[1]==0x00)
				uart_puts("MFOne-S50");
			else if(Temp[0]==0x02&&Temp[1]==0x00)
				uart_puts("MFOne-S70");
			else if(Temp[0]==0x44&&Temp[1]==0x00)
				uart_puts("MF-UltraLight");
			else if(Temp[0]==0x08&&Temp[1]==0x00)
				uart_puts("MF-Pro");
			else if(Temp[0]==0x44&&Temp[1]==0x03)
				uart_puts("MF Desire");
			else
				uart_puts("Unknown");
			if(PcdAnticoll(UID)==MI_OK)
			{
				uart_puts("Card Id is:");
				uart_puthex(UID[0]);
				uart_puthex(UID[1]);
				uart_puthex(UID[2]);
				uart_puthex(UID[3]);

				RED_LED_ON                                            ;
				Delay(200)                                           ;
				RED_LED_OFF                                           ;
				Delay(200)                                           ;
			}
		}
		else GRE_LED_OFF                                            ;
	}
}

void find_card(void)
{
	if(PcdRequest(0x52,Temp)==MI_OK)
	{
		if(Temp[0]==0x04&&Temp[1]==0x00)
			uart_puts("MFOne-S50\n");
		else if(Temp[0]==0x02&&Temp[1]==0x00)
			uart_puts("MFOne-S70\n");
		else if(Temp[0]==0x44&&Temp[1]==0x00)
			uart_puts("MF-UltraLight\n");
		else if(Temp[0]==0x08&&Temp[1]==0x00)
			uart_puts("MF-Pro\n");
		else if(Temp[0]==0x44&&Temp[1]==0x03)
			uart_puts("MF Desire\n");
		else
			uart_puts("Unknown\n");
		uart_puts("Success!\n");
	}
	else
	{
		uart_puts("Failed!\n");
	}
}

void gpio_init(void)
{
}


/********************************************/
void disp_menu(void)
{
	uart_puts("Choose Read or Write operation:\n");
	uart_puts("A-- Automatically find card\n");
	uart_puts("F-- Manually Find card\n");
	uart_puts("*-- Any other key to redispaly the menu\n");
}


/*******************************************
函数名称：exe_cmd
功    能：执行串口收到的命令
参    数：cmd--接收到的来自PC机的命令字符
返回值  ：无
********************************************/
void exe_cmd(char cmd)
{
	switch(cmd)
	{
	case 'a':
	case 'A':
		auto_reader();
		break;
	case 'f':
	case 'F':
		find_card();
		break;
	default:
		disp_menu();
	}
}


void main( void )
{
	gpio_init();
	uart_init(9600);
	uart_puts("OK!\n");
	asm("RIM");
	PcdReset();//复位RC522
	PcdAntennaOn();//开启天线发射
	disp_menu();
	while(1)
	{
		uart_putc('>');
		uart_putc('\n');
		cmd=uart_getc();
		//while(uart_state != UART_OVER);
		exe_cmd(cmd);
	}
}
