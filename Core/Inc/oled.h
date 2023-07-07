#ifndef __OLED_H__
#define __OLED_H__

#include "main.h"
#include "i2c.h"

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_ADDRESS 0x7A //OLED器件地址

//发送一个字节
void OLED_WR_Byte(uint8_t data, uint8_t mode);

//反显函数
void OLED_ColorTurn(uint8_t i);

//屏幕旋转180度
void OLED_DisplayTurn(uint8_t i);

//开启OLED显示
void OLED_DisPlay_On(void);

//关闭OLED显示
void OLED_DisPlay_Off(void);

//更新显存到OLED
void OLED_Refresh(void);

//清屏函数
void OLED_Clear(void);

//OLED的初始化
void OLED_Init();

//画点
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);

// 显示ASCII字符
void OLED_ShowChar(uint8_t x, uint8_t y, char _char, uint8_t size1, uint8_t mode);

// 显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, char *_string, uint8_t size1, uint8_t mode);

// 显示汉字
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1, uint8_t mode);

#endif
