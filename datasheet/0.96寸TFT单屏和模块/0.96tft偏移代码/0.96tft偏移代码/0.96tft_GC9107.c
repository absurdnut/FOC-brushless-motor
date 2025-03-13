///////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：STC51
//0.96寸TFT_ST7735液晶驱动
//版本：V1.0
//////////////////////////////////////////////////////////////////////////////////
#include<reg51.h>
#include<absacc.h>
#include<intrins.h>
#include<string.h>
#include<font.h>
#define uchar unsigned char
#define uint unsigned int

//主频：12MHZ
//单片机IO推挽输出设置定义
//#define USE_LANDSCAPE//测试程序横竖屏切换，横屏请开启此宏，竖屏请屏蔽
#define MCU_STC12//如果您使用的单片机不是STC12系列(如STC89C52)请屏蔽此宏定义
#ifdef MCU_STC12
sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain
#endif

//---------------------------液晶屏接线说明-------------------------------------//
//接线前请参考液晶屏说明书第10页引脚定义
sbit bl        =P3^5;//接模块BLK引脚，背光可以采用IO控制或者PWM控制，也可以直接接到高电平常亮
sbit scl       =P3^0;//接模块SCL引脚,接裸屏Pin9_SCL
sbit sda       =P3^1;//接模块SDA引脚，接裸屏Pin8_SDA
sbit rs        =P3^3;//接模块DC引脚，接裸屏Pin7_A0
sbit cs        =P3^4;//接模块CS引脚，接裸屏Pin12_CS
sbit reset     =P3^2;//接模块RES引脚，接裸屏Pin6_RES
//---------------------------End of液晶屏接线---------------------------------//

//定义常用颜色
#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D
#define GRAY1   0x8410
#define GRAY2   0x4208

void  Contrast_Adjust();

code  typFNT_GBK16 hz16[];

void Output_Pixel(uint x,uint y);

void delay_ms(uint time)
{
 uint i,j;
  for(i=0;i<time;i++)
   for(j=0;j<250;j++);
}

//向SPI总线传输一个8位数据
void  SPI_WriteData(uchar Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)
		sda=1; //输出数据
		else sda=0;
		scl=0;
		scl=1;
		Data<<=1;
	}
}
//向液晶屏写一个8位指令
void  Lcd_WriteIndex(uchar Data)
{

		cs=0;
		rs=0;
		SPI_WriteData(Data);
		cs=1;
}
//向液晶屏写一个8位数据
void  Lcd_WriteData(uchar Data)
{
		unsigned char i=0;
		cs=0;
		rs=1;
		SPI_WriteData(Data);
		cs=1;
}
//向液晶屏写一个16位数据
void  LCD_WriteData_16Bit(unsigned int Data)
{
	unsigned char i=0;
	cs=0;
	rs=1;
	SPI_WriteData(Data>>8); 	//写入高8位数据
	SPI_WriteData(Data); 			//写入低8位数据
	cs=1;

}

void Reset()
{
    reset=0;
    delay_ms(100);
    reset=1;
    delay_ms(100);
}



//////////////////////////////////////////////////////////////////////////////////////////////
//液晶屏初始化
void lcd_initial()

{

	Reset();//Reset before LCD Init.
delay_ms(120);
 Lcd_WriteIndex(0x11);
delay_ms(120);
Lcd_WriteIndex(0x29);
delay_ms(120);	
	
	// Lcd_WriteIndex(0x11);//Sleep exit
	// delay_ms (120);
	// Lcd_WriteIndex(0x21);
// Lcd_WriteIndex(0x21);

// Lcd_WriteIndex(0xB1);
// Lcd_WriteData(0x05);
// Lcd_WriteData(0x3A);
// Lcd_WriteData(0x3A);

// Lcd_WriteIndex(0xB2);
// Lcd_WriteData(0x05);
// Lcd_WriteData(0x3A);
// Lcd_WriteData(0x3A);

// Lcd_WriteIndex(0xB3);
// Lcd_WriteData(0x05);
// Lcd_WriteData(0x3A);
// Lcd_WriteData(0x3A);
// Lcd_WriteData(0x05);
// Lcd_WriteData(0x3A);
// Lcd_WriteData(0x3A);

// Lcd_WriteIndex(0xB4);
// Lcd_WriteData(0x03);

// Lcd_WriteIndex(0xC0);
// Lcd_WriteData(0x62);
// Lcd_WriteData(0x02);
// Lcd_WriteData(0x04);

// Lcd_WriteIndex(0xC1);
// Lcd_WriteData(0xC0);

// Lcd_WriteIndex(0xC2);
// Lcd_WriteData(0x0D);
// Lcd_WriteData(0x00);

// Lcd_WriteIndex(0xC3);
// Lcd_WriteData(0x8D);
// Lcd_WriteData(0x6A);

// Lcd_WriteIndex(0xC4);
// Lcd_WriteData(0x8D);
// Lcd_WriteData(0xEE);

// Lcd_WriteIndex(0xC5);  /*VCOM*/
// Lcd_WriteData(0x0E);

// Lcd_WriteIndex(0xE0);
// Lcd_WriteData(0x10);
// Lcd_WriteData(0x0E);
// Lcd_WriteData(0x02);
// Lcd_WriteData(0x03);
// Lcd_WriteData(0x0E);
// Lcd_WriteData(0x07);
// Lcd_WriteData(0x02);
// Lcd_WriteData(0x07);
// Lcd_WriteData(0x0A);
// Lcd_WriteData(0x12);
// Lcd_WriteData(0x27);
// Lcd_WriteData(0x37);
// Lcd_WriteData(0x00);
// Lcd_WriteData(0x0D);
// Lcd_WriteData(0x0E);
// Lcd_WriteData(0x10);

// Lcd_WriteIndex(0xE1);
// Lcd_WriteData(0x10);
// Lcd_WriteData(0x0E);
// Lcd_WriteData(0x03);
// Lcd_WriteData(0x03);
// Lcd_WriteData(0x0F);
// Lcd_WriteData(0x06);
// Lcd_WriteData(0x02);
// Lcd_WriteData(0x08);
// Lcd_WriteData(0x0A);
// Lcd_WriteData(0x13);
// Lcd_WriteData(0x26);
// Lcd_WriteData(0x36);
// Lcd_WriteData(0x00);
// Lcd_WriteData(0x0D);
// Lcd_WriteData(0x0E);
// Lcd_WriteData(0x10);

// Lcd_WriteIndex(0x3A);
// Lcd_WriteData(0x05);

// Lcd_WriteIndex(0x36);
// Lcd_WriteData(0xC8);

// Lcd_WriteIndex(0x29);




//GC9107F+BOE0.96  VCI=2.8V

Lcd_WriteIndex(0xFE);
Lcd_WriteIndex(0xFE);				
Lcd_WriteIndex(0xEF);
	
Lcd_WriteIndex(0xB0);			
Lcd_WriteData(0xC0); 
	
Lcd_WriteIndex(0xB2);		
Lcd_WriteData(0x24); 
		
Lcd_WriteIndex(0xB3);		
Lcd_WriteData(0x03);
	
Lcd_WriteIndex(0xB7);		
Lcd_WriteData(0x01);  

Lcd_WriteIndex(0xB6);			
Lcd_WriteData(0x19); 
	
Lcd_WriteIndex(0xAC);
Lcd_WriteData(0xDB);
Lcd_WriteIndex(0xAB);
Lcd_WriteData(0x0f);
Lcd_WriteIndex(0x3A);		
Lcd_WriteData(0x05); 
		
Lcd_WriteIndex(0xB4);	
Lcd_WriteData(0x04);

Lcd_WriteIndex(0xA8);	
Lcd_WriteData(0x0C);

Lcd_WriteIndex(0xb8);
Lcd_WriteData(0x08);
 		
Lcd_WriteIndex(0xED);	
Lcd_WriteData(0x03);
		
Lcd_WriteIndex(0xea);
Lcd_WriteData(0x9f); 

Lcd_WriteIndex(0xc6);
Lcd_WriteData(0x2a); 

Lcd_WriteIndex(0xc7);	
Lcd_WriteData(0x10); 

Lcd_WriteIndex(0xF0);	
Lcd_WriteData(0x09);
Lcd_WriteData(0x35);
Lcd_WriteData(0x2a);
Lcd_WriteData(0x4a);
Lcd_WriteData(0xA9);
Lcd_WriteData(0x39);
Lcd_WriteData(0x35);
Lcd_WriteData(0x60);
Lcd_WriteData(0x00);
Lcd_WriteData(0x14);
Lcd_WriteData(0x0a);
Lcd_WriteData(0x16);
Lcd_WriteData(0x10);
Lcd_WriteData(0x1F);

Lcd_WriteIndex(0xF1);	
Lcd_WriteData(0x13);
Lcd_WriteData(0x24);
Lcd_WriteData(0x55);
Lcd_WriteData(0x3c);
Lcd_WriteData(0xc6);
Lcd_WriteData(0x16);
Lcd_WriteData(0x3f);
Lcd_WriteData(0x60);
Lcd_WriteData(0x08);
Lcd_WriteData(0x06);
Lcd_WriteData(0x0d);
Lcd_WriteData(0x1f);
Lcd_WriteData(0x1c);
Lcd_WriteData(0x10);
///////////


/*====================================================*/
// void address(void)
// {             
	// Lcd_WriteIndex(0x2A);       
	// Lcd_WriteData(0x00);  
	// Lcd_WriteData(0x18);  
	// Lcd_WriteData(0x00);
	// Lcd_WriteData(0x67);  

	// Lcd_WriteIndex(0x2B);       
	// Lcd_WriteData(0x00);
	// Lcd_WriteData(0x00);  
	// Lcd_WriteData(0x00);
	// Lcd_WriteData(0x9F); 
				   
	// Lcd_WriteIndex(0x2C);
 // }	
 

}



/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{

	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_start+0x18);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end+0x18);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_start);
	Lcd_WriteData(0x00);
	Lcd_WriteData(y_end);
	Lcd_WriteIndex(0x2c);
}

void PutPixel(uint x_start,uint y_start,uint color)
{
	Lcd_SetRegion(x_start,y_start,x_start+1,y_start+1);
	LCD_WriteData_16Bit(color);

}

void dsp_single_colour(int color)
{
 	uchar i,j;
	Lcd_SetRegion(0,0,80-1,160-1);
 	for (i=0;i<160;i++)
    	for (j=0;j<80;j++)
        	LCD_WriteData_16Bit(color);
}

//采用设置窗口方法填充字库数据，相比Gui_DrawFont_GBK16更快速
void Fast_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s)
{
	unsigned char i,j;
	unsigned short k;
	uint HZnum;
	HZnum=sizeof(hz16)/sizeof(typFNT_GBK16);
	while(*s)
	{
		if((*s) >= 128)
		{
			for (k=0;k<HZnum;k++)
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 	Lcd_SetRegion(x,y,x+16-1,y+16-1);
				    for(i=0;i<16*2;i++)
				    {
						for(j=0;j<8;j++)
				    	{
							if(hz16[k].Msk[i]&(0x80>>j))	LCD_WriteData_16Bit(fc);
							else
							{
							if (fc!=bc) LCD_WriteData_16Bit(bc);
							}
						}
					}


				}
			}
		s+=2;x+=16;
		}


		else
		s+=1;

	}
}
//此方法描绘汉字较慢
void Gui_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;

	while(*s)
	{
		if((*s) >= 128)
		{
			for (k=0;k<hz16_num;k++)
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  {
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++)
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	PutPixel(x+j,y+i,fc);
								else {
									if (fc!=bc) PutPixel(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++)
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	PutPixel(x+j+8,y+i,fc);
								else
								{
									if (fc!=bc) PutPixel(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		}
		else
		s+=1;

	}
}

void Font_Test(void)
{//中文显示测试金逸晨电子欢迎您因为专业所以注
	dsp_single_colour(WHITE);
	Fast_DrawFont_GBK16(8,10,BLUE,YELLOW,	"中文测试");
	Fast_DrawFont_GBK16(0,30,RED,WHITE,	"金逸晨电子");
	Fast_DrawFont_GBK16(16,50,BLUE,WHITE,	"欢迎您");
	Fast_DrawFont_GBK16(8,70,WHITE,BLUE, 	"因为专注");
	Fast_DrawFont_GBK16(8,90,WHITE,BLUE,	"所以专业");

	delay_ms(180);//1800
}
main()
{
#ifdef MCU_STC12
	P3M1 &= ~(1<<2),	P3M0 |=  (1<<2);	//P3.2 set as push-pull output mode
#endif
	lcd_initial();
	bl=1;
  while(1)
  {
	dsp_single_colour(RED);
	dsp_single_colour(GREEN);
	dsp_single_colour(BLUE);
	  
	Font_Test();
    delay_ms(350);//3500
    }
 }

