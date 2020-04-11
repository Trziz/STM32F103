#include "delay.h"
#include "hcsr04.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 #include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "TFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#define Trig PCout(4)		//PC1	TRIG
#define Trig2 PCout(5)		//PC2	TRIG
//函数声明
void UltrasonicWave_Configuration(void);		//配置
void UltrasonicWave_StartMeasure(void);			//测量
void UltrasonicWave_Configuration2(void);		//配置
void UltrasonicWave_StartMeasure2(void);			//测量

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值	
extern u8  TIM5CH2_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH2_CAPTURE_VAL;	//输入捕获值	

float pitch,roll,yaw,Parallel,Vertical,r,d; 		//欧拉角









 int main(void)
 {		
	 //陀螺仪定义
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;					//温度	    
	 u8 report=1;			//默认开启上报
	
	 //超声波定义
	float length;
	u32 tempdis=0; 	
	 u32 tempdis2=0; 
	 float UltrasonicWave_Distance=0;      //计算出的距离
	 float UltrasonicWave_Distance2=0;      //计算出的距离
  SystemInit();	//System init.
  delay_init();//Delay init.
	 uart_init(9600);
	HC_SR04Init();
	  MPU_Init();
	 mpu_dmp_init();
	Test_Demo();
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级

 
	UltrasonicWave_Configuration();
	UltrasonicWave_Configuration2();
 	TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
	 
	 while(mpu_dmp_init())       //6050初始化检测
 	{
		printf("none\n");
	}  
	printf("Yes\n");
   	while(1)
	{
		delay_us(50);
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
				/***俯仰角显示 ***/
				temp=pitch*10;
				if(temp<0)
				{
					Gui_DrawFont_GBK16(10,100,BLACK,GRAY0,"-");
					temp=-temp;		//转为正数
				}else Gui_DrawFont_GBK16(10,100,BLACK,GRAY0," ");
				 shuzi(3,35,100,temp);
				
				printf("--pitch %d--",temp);
				//if()
				
				/***横滚角显示***/
				temp=roll*10;
				if(temp<0)
				{
					Gui_DrawFont_GBK16(50,100,BLACK,GRAY0,"-");
					temp=-temp;		//转为正数
				}else Gui_DrawFont_GBK16(50,100,BLACK,GRAY0," ");
				 shuzi(3,75,100,temp);
				printf("--roll %d--",temp);
				
				/***航向角显示***/
				temp=yaw*10;
				if(temp<0)
				{
					Gui_DrawFont_GBK16(90,100,BLACK,GRAY0,"-");
					temp=-temp;		//转为正数
				}else Gui_DrawFont_GBK16(90,100,BLACK,GRAY0," ");
				 shuzi(3,115,100,temp);
			printf("--yaw %d--\n",temp);
		 
				Vertical=roll;
		  	Parallel=pitch;
		
		}
		
		
		
		
//		
		
		
	{	UltrasonicWave_StartMeasure();//开始测距
		 		 
 		if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			tempdis=TIM5CH1_CAPTURE_STA&0X3F;
			tempdis*=65536;//溢出时间总和
			tempdis+=TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			
			UltrasonicWave_Distance=(float)tempdis*170/10000;//距离计算公式：距离=高电平持续时间*声速/2   
														  //即 距离= 高电平持续时间*340*100/（2*1000 000） = 高电平持续时间*170/10000
			 
			
			shuzi(5,100,35,(int)(UltrasonicWave_Distance));
			TIM5CH1_CAPTURE_STA=0;//开启下一次捕获
		}
	}
	
	length = HC_SR04_GetLength();
	shuzi(5,100,75,(int)(length));
		delay_ms(50);
	
	
		{UltrasonicWave_StartMeasure2();//开始测距
		 		 
 		if(TIM5CH2_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			tempdis2=TIM5CH2_CAPTURE_STA&0X3F;
			tempdis2*=65536;//溢出时间总和
			tempdis2+=TIM5CH2_CAPTURE_VAL;//得到总的高电平时间
			
			UltrasonicWave_Distance2=(float)tempdis2*170/10000;//距离计算公式：距离=高电平持续时间*声速/2   
														  //即 距离= 高电平持续时间*340*100/（2*1000 000） = 高电平持续时间*170/10000
			 
			shuzi(5,100,55,(int)(UltrasonicWave_Distance2));
		
			TIM5CH2_CAPTURE_STA=0;//开启下一次捕获
		}
	}
	
	}
 }


/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					 //PC4接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //初始化外设GPIO 

}
void UltrasonicWave_Configuration2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;					 //PC8接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //初始化外设GPIO 

}
/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_StartMeasure(void)
{
	Trig = 1;
	delay_us(20);		                      //延时20US
	Trig = 0;
	
}
void UltrasonicWave_StartMeasure2(void)
{
	Trig2 = 1;
	delay_us(20);		                      //延时20US
	Trig2 = 0;
	
}

