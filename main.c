#include "stm8s103f.h"
#include "rc522.h"
#include "uart.h"

unsigned char UID[5],Temp[4]                                       ;
unsigned char RF_Buffer[18]                                        ;
unsigned char Password_Buffer[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}   ; // Mifare One 缺省密码

char          MBRX[30]                                             ;
char          MBKeyTP[30]                                          ;
char          event                                                ;
unsigned char DISP_MODE,i                                          ; // 编辑控件显示模式
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
        uart_puts0("Card Id is:");
        tochar(UID[0]);
        tochar(UID[1]);
        tochar(UID[2]);
        tochar(UID[3]);
        while (!(IFG1 & UTXIFG0));
        TXBUF0 = '\n';                              //发送换行指令
                        
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
函数名称：handle_menu_selection
功    能：处理PC的配置函数
参    数：inputvalue--接收到的来自PC机的字符
返回值  ：无
********************************************/
void handle_menu_selection(uchar inputvalue)
{
    switch(toupper(inputvalue)) 
    {
    case 'A':
        auto_reader();
        break;
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
  asm("RIM");
  PcdReset();//复位RC522
  PcdAntennaOn();//开启天线发射 
  disp_menu();
  while(1)
  {
        uart_putc('>');
        uart_putc('\n');
        i=uart_getc();
        handle_menu_selection(i);
  }
}
