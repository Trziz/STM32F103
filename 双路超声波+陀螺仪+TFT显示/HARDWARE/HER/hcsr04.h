#ifndef _HCSR04_H
#define _HCSR04_H

//IO口初始化 及其他初始化
void HC_SR04Init(void);


//通过定时器4计数器值推算距离
float HC_SR04_GetLength(void);

#endif

