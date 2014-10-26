#include "iostm8s103f3.h"
#include "main.h"
#include "rc522.h"
#include "uart.h"
#include "string.h"
#include "timer1.h"
#include "timer2.h"

const char sys_info[72]="RFID Seat Booking Service V1.0\n2014.10 © by xhly: xhlyuestc@gmail.com\n";

unsigned char mf_uid[5],temp[4];                                      ;
unsigned char mf_key_buff[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // Mifare One 缺省密码
unsigned char mf_dat_buff[18];
unsigned char MLastSelectedSnr[4];

char cmd;
unsigned char des_on=0; // DES加密

void find_card(void)
{
	if(PcdRequest(0x52,temp)==MI_OK)
	{
		if(temp[0]==0x04&&temp[1]==0x00)
			uart_puts("MFOne-S50\n");
		else if(temp[0]==0x02&&temp[1]==0x00)
			uart_puts("MFOne-S70\n");
		else if(temp[0]==0x44&&temp[1]==0x00)
			uart_puts("MF-UltraLight\n");
		else if(temp[0]==0x08&&temp[1]==0x00)
			uart_puts("MF-Pro\n");
		else if(temp[0]==0x44&&temp[1]==0x03)
			uart_puts("MF Desire\n");
		else
			uart_puts("Unknown\n");
		if(PcdAnticoll(mf_uid)==MI_OK)
		{
			uart_puts("Card UID is:");
			uart_puthex(mf_uid[0]);
			uart_putc(' ');
			uart_puthex(mf_uid[1]);
			uart_putc(' ');
			uart_puthex(mf_uid[2]);
			uart_putc(' ');
			uart_puthex(mf_uid[3]);
			uart_putc('\n');

			RED_LED_ON;
			timer2_wait_ms(200);
			RED_LED_OFF;
			timer2_wait_ms(200);
		}
	}
	else
	{
		GRE_LED_OFF;
		uart_puts("Failed");
	}
}

void auto_find(void)
{
	while(cmd=='A' || cmd=='a')
	{
		find_card();
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
	char status;
	switch(cmd)
	{
	case 'a':
	case 'A':
		auto_find();
		break;
	case 'f':
	case 'F':
		find_card();
		break;
	case 1: // Halt the card     //终止卡的操作
		status= PcdHalt();;
		ubuff[0]=1;
		ubuff[1]=status;
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case 2: // Request,Anticoll,Select,return CardType(2 bytes)+CardSerialNo(4 bytes)
		// 寻卡，防冲突，选择卡    返回卡类型（2 bytes）+ 卡系列号(4 bytes)
		status= PcdRequest(ubuff[1],&ubuff[2]);
		if(status!=0)
		{
			status= PcdRequest(ubuff[1],&ubuff[2]);
			if(status!=0)
			{
				ubuff[0]=1;
				ubuff[1]=status;
				uart_send_cnt(ubuff,ubuff[0]);
				break;
			}
		}
		ubuff[0]=3;
		ubuff[1]=status;
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case 3: // 防冲突 读卡的系列号 MLastSelectedSnr
		status = PcdAnticoll(&ubuff[2]);
		if(status!=0)
		{
			ubuff[0]=1;
			ubuff[1]=status;
		}
		else
		{
			memcpy(MLastSelectedSnr,&ubuff[2],4);
			ubuff[0]=5;
			ubuff[1]=status;
		}
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case 4:	// 选择卡 Select Card
		status=PcdSelect(MLastSelectedSnr);
		if(status!=MI_OK)
		{
			ubuff[0]=1;
			ubuff[1]=status;
		}
		else
		{
			ubuff[0]=3;
			ubuff[1]=status;
		}
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case 5:	// Key loading into the MF RC500's EEPROM
		status = PcdAuthState(ubuff[1], ubuff[3], mf_key_buff, MLastSelectedSnr);// 校验卡密码
	case 6:
	case 7:
		ubuff[0]=1;
		ubuff[1]=status;
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case 8: // Read the mifare card
		// 读卡
		status=PcdRead(ubuff[1],&ubuff[2]);
		if(status==0)
		{
			ubuff[0]=17;
		}
		else
		{
			ubuff[0]=1;
		}
		ubuff[1]=status;
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case 9: // Write the mifare card
		// 写卡  下载密码
		status=PcdWrite(ubuff[1],&ubuff[2]);
		ubuff[0]=1;
		ubuff[1]=status;
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case 10:
		PcdValue(ubuff[1],ubuff[2],&ubuff[3]);
		ubuff[0]=1;
		ubuff[1]=status;
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case 12: // 参数设置
		PcdBakValue(ubuff[1], ubuff[2]);
		ubuff[0]=1;	//contact
		ubuff[1]=0;
		uart_send_cnt(ubuff,ubuff[0]);
		break;
	case CMD_SYS_INFO:
		uart_puts(sys_info);
	case CMD_SYS_RST:
		sys_reset();
	case CMD_SLEEP:
		mcu_sleep();
	default:
		disp_menu();
	}
}


void main( void )
{
	gpio_init();
	uart_init(9600);
	uart_puts("OK!\n");
	timer1_init();
	timer2_init();
	asm("RIM");
	timer1_start();
	PcdReset();//复位RC522
	PcdAntennaOn();//开启天线发射
	disp_menu();
	while(1)
	{
		uart_puts(">\n");
		cmd=uart_getc();
		//while(uart_state != UART_OVER);
		exe_cmd(cmd);
	}
}

void sys_reset(void)
{
	asm("SIM");//关全局中断
	//asm("JPF 08000H");//跳到复位中断向量处
	WWDG_CR=0X80;//开启Window Watch Dog并清除T6位来复位
	while(1);
}
