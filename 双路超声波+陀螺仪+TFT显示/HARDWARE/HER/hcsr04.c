#include "hcsr04.h"
#include "delay.h"


//超声波计数
u16 msHcCount = 0;


//定时器2设置
void HC_SR04_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       
	NVIC_Init(&NVIC_InitStructure);
}


//IO口初始化 及其他初始化
void HC_SR04Init(void)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_5);
     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);    
     
          
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   
     
    TIM_TimeBaseStructure.TIM_Period =1000-1; 
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);          
        
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);  
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
    HC_SR04_NVIC();
    TIM_Cmd(TIM2,DISABLE);     
}


//打开定时器2
static void OpenTimerForHc(void)  
{
   TIM_SetCounter(TIM2,0);
   msHcCount = 0;
   TIM_Cmd(TIM2, ENABLE); 
}


//关闭定时器2
static void CloseTimerForHc(void)    
{
   TIM_Cmd(TIM2, DISABLE); 
}


//定时器2终中断
void TIM2_IRQHandler(void)  
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
   {
       TIM_ClearITPendingBit(TIM2, TIM_IT_Update  ); 
       msHcCount++;
   }
}

//获取定时器2计数器值
u32 GetEchoTimer(void)
{
   u32 t = 0;
   t = msHcCount*1000;
   t += TIM_GetCounter(TIM2);
   TIM2->CNT = 0;  
   delay_ms(50);
   return t;
}


//通过定时器2计数器值推算距离
float HC_SR04_GetLength(void )
{
   u32 t = 0;
   int i = 0;
   float lengthTemp = 0;
      GPIO_SetBits(GPIOB,GPIO_Pin_5);     
      delay_us(20);
      GPIO_ResetBits(GPIOB,GPIO_Pin_5);
      while(PBin(6) == 0);      
      OpenTimerForHc();        
      i++;
      while(PBin(6) == 1);
      CloseTimerForHc();        
      t = GetEchoTimer();        
      lengthTemp = ((float)t/58.0);//cm
    return lengthTemp;
}


