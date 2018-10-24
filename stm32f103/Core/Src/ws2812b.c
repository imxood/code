#include "ws2812b.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <math.h>

#define CommandLen 5

volatile uint8_t ColorBuffer[LED_BUFFER_SIZE];
volatile uint8_t CommandBuffer[CommandLen];

volatile uint8_t LedType = 1;

volatile uint8_t cmdReceived = 0; // 是否接收到命令

// ws2812 模块的外设使能初始化
void ws2812_init() {
	// TIM4 Channel2
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH2); // 使能通道
	LL_TIM_EnableDMAReq_CC2(TIM4);					   // 使能DMA请求
	LL_TIM_EnableCounter(TIM4);						   // 使能计数器, 即开始计数

	// DMA1 Channel4
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t) (&TIM4->CCR2)); // 设置外设地址
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t) ColorBuffer); // 设置DMA的数据地址
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, LED_BUFFER_SIZE);// 设置DMA的数据长度

	// USART1
	LL_USART_EnableDMAReq_RX(USART1); // 允许DMA请求
	LL_USART_EnableIT_IDLE(USART1);   // 使能IDLE中断

	// DMA1 Channel5
	// 串口DMA请求: 使能dma一帧数据传输完成-中断
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t) (&USART1->DR)); // 设置外设地址
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t) CommandBuffer); // 设置DMA的数据地址
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, CommandLen);	// 设置DMA的数据长度

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
}

Color ToColor(uint8_t r, uint8_t g, uint8_t b) {
	Color color = {r, g, b, 100};
	return color;
}

Color ToColorA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	Color color = {r, g, b, a};
	return color;
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

void ledControlRequest(void) {
	if (LL_USART_IsActiveFlag_IDLE(USART1)) {

		uint8_t len = LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_5);
		if (len == 0) {
			//
			if (CommandBuffer[0] == 0xff && CommandBuffer[1] == 0xff
					&& CommandBuffer[3] == 0x0d && CommandBuffer[4] == 0x0a) {
				// memcpy((void *)Command, (void *)CommandBuffer, CommandLen);
				LedType = CommandBuffer[2];
				cmdReceived = 1; // 接收到控制命令
			}
		}

		// reset the number of data to transfer
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
		LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, CommandLen);
		LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);

		LL_USART_ClearFlag_IDLE(USART1);
	}
}

void run(osSemaphoreId ledCtrlSemHandle) {

	ws2812_init();

	colorWipe(ToColor(255, 0, 0), 500);

	while (1) {
		//		osSemaphoreWait(ledCtrlSemHandle, portMAX_DELAY);

		if (cmdReceived == 1) {

			switch (LedType) {
			// 0, 白灯微闪烁  1, 白灯  2, 红灯  3, 绿灯  4, 蓝灯
			case 0:
				break;
			case 1:
				colorWipe(ToColor(255, 255, 255), 5);
				break;
			case 2:
				colorWipe(ToColor(255, 0, 0), 5);
				break;
			case 3:
				colorWipe(ToColor(0, 255, 0), 5);
				break;
			case 4:
				colorWipe(ToColor(0, 0, 255), 5);
				break;
			case 5:
				break;
			}

			cmdReceived = 0;
		} else {
			osDelay(50);
		}

		//		osSemaphoreRelease(ledCtrlSemHandle);

		// Send a theater pixel chase in...
		//		theaterChase(Color(127, 127, 127), 50); // White
		//		theaterChase(Color(127, 0, 0), 50);		// Red
		//		theaterChase(Color(0, 0, 127), 50);		// Blue
		//
		//		rainbow(20);			 //彩虹
		//		rainbowCycle(20);		 //循环
		//		theaterChaseRainbow(50); //呼吸灯
	}
}

// 设置一个灯的颜色, G.R.B, 有3个字节, 共24个bit, 把这24个bit, 按G,R,B的顺序, 且每种颜色有高位到低位, 取出每一位,
// 为0则为WS2812_0, 为1则为WS2812_1, 得出的24个WS2812编码值, 即为timer的24个输出Pulse值
// 由此输出24个脉冲, timer的计数频率是:72Mhz / 90 = 800Khz, 则每一个脉冲间隔是1.25us, 就这样实现WS2812的脉冲调制
void SetPixelColor(uint16_t n, Color color) {
	uint8_t tempBuffer[24];
	uint8_t i;

	for (i = 0; i < 8; i++) // G
		tempBuffer[i] = ((color.g << i) & 0x80) ? WS2812_1 : WS2812_0;

	for (i = 0; i < 8; i++) // R
		tempBuffer[8 + i] = ((color.r << i) & 0x80) ? WS2812_1 : WS2812_0;

	for (i = 0; i < 8; i++) // B
		tempBuffer[16 + i] = ((color.b << i) & 0x80) ? WS2812_1 : WS2812_0;

	memcpy((void *) (RESET_SLOTS_BEGIN + n * 24), (void*) tempBuffer, 24);
}

void PixelUpdate() {
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, LED_BUFFER_SIZE);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}

// Fill the dots one after the other with a color
void colorWipe(Color color, uint8_t wait) {
	uint16_t i = 0;
	for (i = 0; i < LED_NUMBER; i++) {
		SetPixelColor(i, color);
		PixelUpdate();
		osDelay(wait);
	}
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
