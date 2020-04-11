/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "delay.h"

#include "TFT_demo.h"
unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};
void shuzi(int i,int x,int y,int shuju)
{  
	 char biaozhi=0,c;
	 char *s;
	 for(c=0;c<i;c++)
	 {
	   biaozhi=shuju%10;
		 shuju=shuju/10;
		 switch(biaozhi+1)
		 {
			case 10:    s="9";   break;
			case 9:     s="8";   break;
			case 8:     s="7";   break;
			case 7:     s="6";   break;
			case 6:     s="5";   break;
			case 5:     s="4";   break;
			case 4:     s="3";   break;
			case 3:     s="2";   break;
			case 2:     s="1";   break;
			default:    s="0"; 
		 }
	   Gui_DrawFont_GBK16(x,y,BLACK,GRAY0,s);
		 x=x-8;
	 } 
	   
	
}

void Redraw_Mainmenu(void)
{

	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(8,0,BLUE,GRAY0,"STM32电子技术");
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"液晶测试程序");

	DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,40,YELLOW,GRAY0,"颜色填充测试");

	DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,70,BLUE,GRAY0,"文字显示测试");

	DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,100,RED,GRAY0,"图片显示测试");
	delay_ms(1500);
}

void Num_Test(void)
{
	u8 i=0;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	Lcd_Clear(GRAY0);

	for(i=0;i<10;i++)
	{
	Gui_DrawFont_Num32((i%3)*40,32*(i/3)+5,RED,GRAY0,Num[i+1]);
	delay_ms(100);
	}
	
}

void Font_Test(void)
{

	Lcd_Clear(GRAY0);
//	delay_ms(1000);
	
	Gui_DrawFont_GBK16(30,8,BLACK,GRAY0,"DISTANCE");
	Gui_DrawFont_GBK16(12,35,BLACK,GRAY0,"LEFT :");
	Gui_DrawFont_GBK16(12,55,BLACK,GRAY0,"FRONT:");
	Gui_DrawFont_GBK16(12,75,BLACK,GRAY0,"RIGHT:");

//	Gui_DrawFont_GBK16(16,48,RED,GRAY0, "全程技术支持");
//	Gui_DrawFont_GBK16(0,68,BLUE,GRAY0," Tel:1234567890");
//	Gui_DrawFont_GBK16(0,88,RED,GRAY0, " mcudev.taobao");	
	delay_ms(1800);	
}

void Color_Test(void)
{
	u8 i=1;
	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(20,10,BLUE,GRAY0,"Color Test");
	delay_ms(500);

	while(i--)
	{
		Lcd_Clear(WHITE);
		delay_ms(500);
		Lcd_Clear(BLACK);
		delay_ms(500);
		Lcd_Clear(RED);
		delay_ms(500);
	  Lcd_Clear(GREEN);
		delay_ms(500);
	  Lcd_Clear(BLUE);
		delay_ms(500);
	}		
}

//取模方式 水平扫描 从左到右 低位在前
void showimage(const unsigned char *p) //显示40*40 QQ图片
{
  	int i,j,k; 
	unsigned char picH,picL;
	Lcd_Clear(WHITE); //清屏  
	
	for(k=0;k<3;k++)
	{
	   	for(j=0;j<3;j++)
		{	
			Lcd_SetRegion(40*j,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//数据低位在前
				picH=*(p+i*2+1);				
				LCD_WriteData_16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}


//取模方式 水平扫描 从左到右 低位在前
void Fullscreen_showimage(const unsigned char *p) //显示128*128 图片
{
  	int i,j,k; 
	unsigned char picH,picL;
	
	Lcd_Clear(WHITE); //清屏  
	
	
			Lcd_SetRegion(0,0,127,127);		//坐标设置:扫描起点到终点0到127，刚好128个点
		    for(i=0;i<128*128;i++)
				 {	
					picL=*(p+i*2);	//数据低位在前
					picH=*(p+i*2+1);				
					LCD_WriteData_16Bit(picH<<8|picL);  						
				 }	
		
}


void Test_Demo(void)
{
	Lcd_Init();
//	LCD_LED_SET;//通过IO控制背光亮				:例程不需要，注释掉
//	Redraw_Mainmenu();//绘制主菜单(部分内容由于分辨率超出物理值可能无法显示)
//	Color_Test();//简单纯色填充测试
//	Num_Test();//数码管字体测试
	Font_Test();//中英文显示测试		
//	
//	showimage(gImage_qq);//图片显示示例:显示示例的图片越大，会占用更多的FLASH空间，可以根据情况适当减少图片显示数量
//	
//	delay_ms(1500);
//	
//	Fullscreen_showimage(gImage_XHR128);//图片显示示例:显示示例的图片越大，会占用更多的FLASH空间，可以根据情况适当减少图片显示数量
//	delay_ms(1500);
//	Fullscreen_showimage(gImage_XNH128);//图片显示示例
//	delay_ms(1500);
//	Fullscreen_showimage(gImage_ATM128);//图片显示示例
//	delay_ms(1500);
}
