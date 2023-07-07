#include "oled.h"
#include "oledfont.h"

uint8_t OLED_GRAM[144][8];

//发送一个字节
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t data, uint8_t mode) {
	uint8_t send_buf[2] = { 0 };
	if (mode)
		send_buf[0] = 0x40;
	else
		send_buf[0] = 0x00;
	send_buf[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, send_buf, 2, 1);
}

//反显函数
void OLED_ColorTurn(uint8_t i) {
	if (i == 0) {
		OLED_WR_Byte(0xA6, OLED_CMD); //正常显示
	}
	if (i == 1) {
		OLED_WR_Byte(0xA7, OLED_CMD); //反色显示
	}
}

//屏幕旋转180度
void OLED_DisplayTurn(uint8_t i) {
	if (i == 0) {
		OLED_WR_Byte(0xC8, OLED_CMD); //正常显示
		OLED_WR_Byte(0xA1, OLED_CMD);
	}
	if (i == 1) {
		OLED_WR_Byte(0xC0, OLED_CMD); //反转显示
		OLED_WR_Byte(0xA0, OLED_CMD);
	}
}

//开启OLED显示
void OLED_DisPlay_On(void) {
	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵使能
	OLED_WR_Byte(0x14, OLED_CMD); //开启电荷泵
	OLED_WR_Byte(0xAF, OLED_CMD); //点亮屏幕
}

//关闭OLED显示
void OLED_DisPlay_Off(void) {
	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵使能
	OLED_WR_Byte(0x10, OLED_CMD); //关闭电荷泵
	OLED_WR_Byte(0xAE, OLED_CMD); //关闭屏幕
}

//更新显存到OLED
void OLED_Refresh(void) {
	uint8_t i, n;
	uint8_t send_buf[129] = { 0 };
	for (i = 0; i < 8; i++) {
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
		OLED_WR_Byte(0x02, OLED_CMD);   //设置低列起始地址
		OLED_WR_Byte(0x10, OLED_CMD);   //设置高列起始地址
		send_buf[0] = 0x40;
		for (n = 0; n < 128; n++) {
			send_buf[n + 1] = OLED_GRAM[n][i];
		}
		HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, send_buf, 129, 20);
	}
}

//清屏函数
void OLED_Clear(void) {
	uint8_t i, n;
	for (i = 0; i < 8; i++) {
		for (n = 0; n < 128; n++) {
			OLED_GRAM[n][i] = 0;   //清除所有数据
		}
	}
//	OLED_Refresh();   //更新显示
}

//OLED的初始化
void OLED_Init(void) {
	OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
	OLED_WR_Byte(0x02, OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x40, OLED_CMD); /*set display start line*/
	OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/

	OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
	OLED_WR_Byte(0xcf, OLED_CMD); /*128*/

	OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
	OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
	OLED_WR_Byte(0x3F, OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xad, OLED_CMD); /*set charge pump enable*/
	OLED_WR_Byte(0x8b, OLED_CMD); /* 0x8B 内供 VCC */
	OLED_WR_Byte(0x33, OLED_CMD); /*0X30---0X33 set VPP 9V */
	OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
	OLED_WR_Byte(0x00, OLED_CMD); /* 0x20 */

	OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
	OLED_WR_Byte(0x80, OLED_CMD);

	OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
	OLED_WR_Byte(0x1f, OLED_CMD); /*0x22*/

	OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
	OLED_WR_Byte(0x12, OLED_CMD);
	
	OLED_WR_Byte(0xdb, OLED_CMD); /*set vcomh*/
	OLED_WR_Byte(0x40, OLED_CMD);

	OLED_Clear();
	OLED_Refresh();
	OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t) {
	uint8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	if (t)
		OLED_GRAM[x][i] |= n;
	else {
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	}
}

/**
 * @brief: 在指定位置显示一个ASCII字符
 * x: 0~127
 * y: 0~63
 * _char: 要显示的字符
 * size1: 字号8(6x8)/12(6x12)/16(8x16)/24(12x24)
 * mode: 0反色显示, 1正常显示
 *
 * */
void OLED_ShowChar(uint8_t x, uint8_t y, char _char, uint8_t size1, uint8_t mode) {
	uint8_t i, m, temp, size2, chr1;
	uint8_t x0 = x, y0 = y;
	if (size1 == 8)
		size2 = 6;
	else
		size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); //得到字体一个字符对应点阵集所占的字节数
	chr1 = _char - ' ';  //计算偏移后的值
	for (i = 0; i < size2; i++) {
		if (size1 == 8)
			temp = asc2_0806[chr1][i]; //调用0806字体
		else if (size1 == 12)
			temp = asc2_1206[chr1][i]; //调用1206字体
		else if (size1 == 16)
			temp = asc2_1608[chr1][i]; //调用1608字体
		else if (size1 == 24)
			temp = asc2_2412[chr1][i]; //调用2412字体
		else
			return;
		for (m = 0; m < 8; m++) {
			if (temp & 0x01)
				OLED_DrawPoint(x, y, mode);
			else
				OLED_DrawPoint(x, y, !mode);
			temp >>= 1;
			y++;
		}
		x++;
		if ((size1 != 8) && ((x - x0) == size1 / 2)) {
			x = x0;
			y0 = y0 + 8;
		}
		y = y0;
	}
}

/**
 * @brief: 在指定位置显示字符串
 * x: 0~127
 * y: 0~63
 * _string: 要显示的字符串
 * size1: 字号8(6x8)/12(6x12)/16(8x16)/24(12x24)
 * mode: 0反色显示, 1正常显示
 *
 * */
void OLED_ShowString(uint8_t x, uint8_t y, char *_string, uint8_t size1, uint8_t mode) {
	while ((*_string >= ' ') && (*_string <= '~')) //判断是不是非法字符!
	{
		OLED_ShowChar(x, y, *_string, size1, mode);
		if (size1 == 8)
			x += 6;
		else
			x += size1 / 2;
		_string++;
	}
}

/**
 * @brief: 在指定位置显示中文字符
 * x: 0~127
 * y: 0~63
 * num: 要显示的字符串
 * size1: 字号16(16x16)
 * mode: 0反色显示, 1正常显示
 *
 * */
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1, uint8_t mode) {
	uint8_t m, temp;
	uint8_t x0 = x, y0 = y;
	uint16_t i, size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1;  //得到字体一个字符对应点阵集所占的字节数
	for (i = 0; i < size3; i++) {
		if (size1 == 16) {
			temp = Hzk1[num][i];
		}  //调用16*16字体
		for (m = 0; m < 8; m++) {
			if (temp & 0x01)
				OLED_DrawPoint(x, y, mode);
			else
				OLED_DrawPoint(x, y, !mode);
			temp >>= 1;
			y++;
		}
		x++;
		if ((x - x0) == size1) {
			x = x0;
			y0 = y0 + 8;
		}
		y = y0;
	}
}
