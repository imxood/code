#include <stdint.h>

#include <string.h>
#include <math.h>

#include "cmsis_os.h"
#include "main.h"

#include "ws2812b.h"
#include "command.h"

volatile uint8_t CommandAcked = 1; // 已确认

volatile uint8_t ColorPulses[LED_BUFFER_SIZE]; //

volatile uint8_t CommandReceived = 0; // 是否接收到命令
extern uint8_t CommandBuffer[CommandMaxLen]; // 接收到的命令缓冲区

volatile uint8_t LedType = 1; // led的类型

ColorCommand colorCommand = { NULL, 0, 0, 0 }; // 保存解析到的灯带控制命令     --------内部的colors是堆中内存,需要手动分配与释放

// ws2812 模块的外设使能初始化
void ws2812_init() {
	// TIM4 Channel2
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH2); // 使能通道
	LL_TIM_EnableDMAReq_CC2(TIM4);					   // 使能DMA请求
	LL_TIM_EnableCounter(TIM4);						   // 使能计数器, 即开始计数

	// DMA1 Channel4
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t) (&TIM4->CCR2)); // 设置外设地址
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t) ColorPulses); // 设置DMA的数据地址
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, LED_BUFFER_SIZE); // 设置DMA的数据长度

	// USART1
	LL_USART_EnableDMAReq_RX(USART1); // 允许DMA请求
	LL_USART_EnableIT_IDLE(USART1);   // 使能IDLE中断

	// DMA1 Channel5
	// 串口DMA请求: 使能dma一帧数据传输完成-中断
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t) (&USART1->DR)); // 设置外设地址
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t) CommandBuffer); // 设置DMA的数据地址
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, CommandMaxLen); // 设置DMA的数据长度

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
}

void PixelUpdate() {
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, LED_BUFFER_SIZE);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}

void run() {

	ws2812_init();

//	rainbowCycle(5);

	while (1) {

		if (!CommandAcked) { // 检测是否需要发送确认
			SendAckData();
		}

		if (CommandReceived == 1) { // 新的控制

			CommandReceived = 0;

			linearColor();

//			switch (LedType) {
//			// 0, 白灯微闪烁  1, 白灯  2, 红灯  3, 绿灯  4, 蓝灯
//			case 0:
//				white_gradient(10);
//				break;
//			case 1:
//				colorWipe(ToColor(255, 255, 255), 5);
//				break;
//			case 2:
//				colorWipe(ToColor(255, 0, 0), 5);
//				break;
//			case 3:
//				colorWipe(ToColor(0, 255, 0), 5);
//				break;
//			case 4:
//				colorWipe(ToColor(0, 0, 255), 5);
//				break;
//			case 5:
//				rainbow(50);
//				break;
//			case 6:
//				rainbowCycle(20);
//				break;
//			case 7:
//				theaterChase(ToColor(0, 0, 255), 50);
//				break;
//			}

		} else {
			osDelay(50);
		}
	}
}

/**
 * 控制一个周期的颜色变化
 * colors: 颜色数组
 * count: 颜色数组长度
 * rate: 速度，一个周期用的时间
 * delay: 延时，一个周期之后延时时间
 */
void linearColor() {

	static uint8_t headPos; // 当前位置
	static uint8_t* colorPulsesBuf = NULL; // 用于保存命令解析到的脉冲波
	uint8_t i;

	if (colorPulsesBuf) {
		free(colorPulsesBuf);
		colorPulsesBuf = NULL;
	}

	colorPulsesBuf = (uint8_t*) malloc(colorCommand.colorSize * 24); // 3(RGB) * 8(BIT)

	// 计算要输出的脉冲信号
	for (uint8_t j = 0; j < colorCommand.colorSize; j++) {

		Color color = colorCommand.colors[j];

		for (i = 0; i < 8; i++) // G
			colorPulsesBuf[j * 24 + i] =
					((color.g << i) & 0x80) ? WS2812_1 : WS2812_0;

		for (i = 0; i < 8; i++) // R
			colorPulsesBuf[j * 24 + 8 + i] =
					((color.r << i) & 0x80) ? WS2812_1 : WS2812_0;

		for (i = 0; i < 8; i++) // B
			colorPulsesBuf[j * 24 + 16 + i] =
					((color.b << i) & 0x80) ? WS2812_1 : WS2812_0;
	}

	while (!CommandReceived) { // 如果接收到新的控制信号, 就跳出循环

		headPos = 0; // 灯带头部初始位置是0

		while (!CommandReceived && headPos != LED_NUMBER) {

			memset(ColorPulses + RESET_SLOTS_BEGIN, WS2812_RESET,
			LED_DATA_SIZE);

			PixelUpdate();

			osDelay(1);

			memcpy(ColorPulses + RESET_SLOTS_BEGIN + headPos * 24,
					colorPulsesBuf, colorCommand.colorSize * 24);

			PixelUpdate();

			// 延时期间, DMA执行直到数据长度为0时自动关闭
			// 灯带频率为800k, 那么灯带执行一次状态的时间为: 1/800k * LED_BUFFER_SIZE ≈ 1ms
			osDelay(1 + colorCommand.interval);

			headPos++;
		}
		if (!CommandReceived) {
			osDelay(colorCommand.delay);
		}
	}

//	memcpy((void *) (ColorPulses + RESET_SLOTS_BEGIN + i * 24), (void*) tempBuffer, 24);
}

void white_gradient(uint8_t wait) {

	const uint8_t DELTA_MIN = 0, DELTA_MAX = 60; // 亮度变化的范围: DELTA_MIN ~ DELTA_MAX

	uint8_t alpha = 100, direction = 1; // 初始亮度: 100  亮度变化方向: 0表示变化值递减, 1表示变化值递增

	uint16_t delta = 0;						// 初始亮度的变化值
	Color colora = { 255, 255, 255, 100 };	// 初始颜色
	Color color;

	while (!CommandReceived) { // j 控制每次亮度的范围
		colora.a = alpha - delta;
		color = ColorA2Color(colora);

		allColor(color, wait);

		if (direction) {
			delta++;
			if (delta >= DELTA_MAX) {
				direction = 0;
			}
		} else {
			delta--;
			if (delta <= DELTA_MIN) {
				direction = 1;
			}
		}
	}
}

Color ToColor(uint8_t r, uint8_t g, uint8_t b) {
	Color color = { r, g, b, 100 };
	return color;
}

Color ToColorA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	Color color = { r, g, b, a };
	return color;
}

Color ColorA2Color(Color colora) {
	// Hue 色度, Lightness 亮度, Saturation 饱和度。
	double h, l, s;
	RGB2HSL(colora, &h, &l, &s);
	l = (double) colora.a / 100 * l; // 新的透明度是原来透明度的百分比例
	HSL2RGB(h, l, s, &colora);
	colora.a = 100;
	return colora;
}

void RGB2HSL(Color color, double *h, double *l, double *s) {
	double dr = (double) color.r / 255;
	double dg = (double) color.g / 255;
	double db = (double) color.b / 255;
	double cmax = fmax(dr, fmax(dg, db));
	double cmin = fmin(dr, fmin(dg, db));
	double cdes = cmax - cmin;
	double hh, ll, ss;

	ll = (cmax + cmin) / 2;
	if (cdes) {
		if (ll <= 0.5)
			ss = (cmax - cmin) / (cmax + cmin);
		else
			ss = (cmax - cmin) / (2 - cmax - cmin);

		if (cmax == dr)
			hh = (0 + (dg - db) / cdes) * 60;
		else if (cmax == dg)
			hh = (2 + (db - dr) / cdes) * 60;
		else
			// if(cmax == b)
			hh = (4 + (dr - dg) / cdes) * 60;
		if (hh < 0)
			hh += 360;
	} else
		hh = ss = 0;

	*h = hh;
	*l = ll;
	*s = ss;
}

void HSL2RGB(double h, double l, double s, Color* color) {
	double cmax, cmin;

	if (l <= 0.5)
		cmax = l * (1 + s);
	else
		cmax = l * (1 - s) + s;

	cmin = 2 * l - cmax;

	if (s == 0) {
		color->r = color->g = color->b = l * 255;
	} else {
		color->r = HSL2RGBvalue(cmin, cmax, h + 120) * 255;
		color->g = HSL2RGBvalue(cmin, cmax, h) * 255;
		color->b = HSL2RGBvalue(cmin, cmax, h - 120) * 255;
	}
}

double HSL2RGBvalue(double n1, double n2, double hue) {
	if (hue > 360)
		hue -= 360;
	else if (hue < 0)
		hue += 360;
	if (hue < 60)
		return n1 + (n2 - n1) * hue / 60;
	else if (hue < 180)
		return n2;
	else if (hue < 240)
		return n1 + (n2 - n1) * (240 - hue) / 60;
	else
		return n1;
}

// 设置一个灯的颜色, G.R.B, 有3个字节, 共24个bit, 把这24个bit, 按G,R,B的顺序, 且每种颜色有高位到低位, 取出每一位,
// 为0则为WS2812_0, 为1则为WS2812_1, 得出的24个WS2812编码值, 即为timer的24个输出Pulse值
// 由此输出24个脉冲, timer的计数频率是:72Mhz / 90 = 800Khz, 则每一个脉冲间隔是1.25us, 就这样实现WS2812的脉冲调制
void SetPixelColor(uint16_t ledIndex, Color color) {
	uint8_t tempBuffer[24];
	uint8_t i;

	for (i = 0; i < 8; i++) // G
		tempBuffer[i] = ((color.g << i) & 0x80) ? WS2812_1 : WS2812_0;

	for (i = 0; i < 8; i++) // R
		tempBuffer[8 + i] = ((color.r << i) & 0x80) ? WS2812_1 : WS2812_0;

	for (i = 0; i < 8; i++) // B
		tempBuffer[16 + i] = ((color.b << i) & 0x80) ? WS2812_1 : WS2812_0;

	memcpy((void *) (ColorPulses + RESET_SLOTS_BEGIN + ledIndex * 24),
			(void*) tempBuffer, 24);
}

// Fill the dots one after the other with a color
void colorWipe(Color color, uint8_t wait) {
//	static uint8_t
	for (uint16_t i = 0; i < LED_NUMBER; i++) {
		SetPixelColor(i, color);
		PixelUpdate();
		osDelay(wait);
	}
}

void allColor(Color color, uint8_t wait) {
	for (uint16_t i = 0; i < LED_NUMBER; i++) {
		SetPixelColor(i, color);
	}
	PixelUpdate();
	osDelay(wait);
}

Color Wheel(uint8_t WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return ToColor(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return ToColor(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return ToColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//彩虹
void rainbow(uint8_t wait) {
	uint16_t i, j;

	for (j = 0; j < 256; j++) {
		for (i = 0; i < LED_NUMBER; i++) {
			SetPixelColor(i, Wheel((i + j) & 255));
			PixelUpdate();
			osDelay(wait);
		}
	}
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
	uint16_t i, j;

	for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
		for (i = 0; i < LED_NUMBER; i++) {
			SetPixelColor(i, Wheel(((i * 256 / LED_NUMBER) + j) & 255));
		}
		PixelUpdate();
		osDelay(wait);
	}
}

//Theatre-style crawling lights.呼吸灯
void theaterChase(Color color, uint8_t wait) {
	for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
		for (int q = 0; q < 3; q++) {
			for (uint16_t i = 0; i < LED_NUMBER; i = i + 3) {
				SetPixelColor(i + q, color); //turn every third pixel on
			}
			PixelUpdate();
			osDelay(wait);

			for (uint16_t i = 0; i < LED_NUMBER; i = i + 3) {
				SetPixelColor(i + q, ToColor(0, 0, 0)); //turn every third pixel off
			}
			PixelUpdate();
			osDelay(wait);
		}
	}
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
	for (int j = 0; j < 256; j++) { // cycle all 256 colors in the wheel
		for (int q = 0; q < 3; q++) {
			for (uint16_t i = 0; i < LED_NUMBER; i = i + 3) {
				SetPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
				PixelUpdate();
				osDelay(wait);
			}

			for (uint16_t i = 0; i < LED_NUMBER; i = i + 3) {
				SetPixelColor(i + q, ToColor(0, 0, 0)); //turn every third pixel off
				PixelUpdate();
				osDelay(wait);
			}
		}
	}
}

void led_control(const uint8_t* cmd, uint8_t commandLen) {

	if (colorCommand.colors) {
		free(colorCommand.colors);
	}
	colorCommand.colorSize = cmd[8];
	colorCommand.colors = (uint8_t*) malloc(colorCommand.colorSize * 4);

	for (int i = 0; i < colorCommand.colorSize; i++) {
		colorCommand.colors[i].r = cmd[9 + i * 3 + 0];
		colorCommand.colors[i].g = cmd[9 + i % 3 + 1];
		colorCommand.colors[i].b = cmd[9 + i % 3 + 2];
		colorCommand.colors[i].a = 100;
	}

	//memcpy(colorCommand.colors, cmd + 9, colorCommand.colorSize * 3);

	colorCommand.interval = cmd[0] + (cmd[1] << 8) + (cmd[2] << 16)
			+ (cmd[3] << 24);
	colorCommand.delay = cmd[4] + (cmd[5] << 8) + (cmd[6] << 16)
			+ (cmd[7] << 24);

	CommandReceived = 1;

}
