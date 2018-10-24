#include "ws2812b.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

#define CommandLen		5

volatile uint8_t ColorBuffer[LED_BUFFER_SIZE];
volatile uint8_t CommandBuffer[CommandLen];

volatile uint8_t LedType = 1;

volatile uint8_t cmdReceived = 0; // 是否接收到命令

// ws2812 模块的外设使能初始化
void ws2812_init() {
	// TIM4 Channel2
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH2); // 使能通道
	LL_TIM_EnableDMAReq_CC2(TIM4); //使能DMA请求
	LL_TIM_EnableCounter(TIM4); //使能计数器, 即开始计数

	// DMA1 Channel4
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t) (&TIM4->CCR2)); // 设置外设地址
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t) ColorBuffer); // 设置DMA的数据地址
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, LED_BUFFER_SIZE); // 设置DMA的数据长度

	// USART1
	LL_USART_EnableDMAReq_RX(USART1); // 允许DMA请求
	LL_USART_EnableIT_IDLE(USART1); // 使能IDLE中断

	// DMA1 Channel5
	// 串口DMA请求: 使能dma一帧数据传输完成-中断
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t) (&USART1->DR)); // 设置外设地址
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t) CommandBuffer); // 设置DMA的数据地址
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, CommandLen); // 设置DMA的数据长度

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);

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

	colorWipe(Color(255, 255, 255), 500);

	while (1) {
//		osSemaphoreWait(ledCtrlSemHandle, portMAX_DELAY);

		if (cmdReceived == 1) {

			switch (LedType) {
			// 0, 白灯微闪烁  1, 白灯  2, 红灯  3, 绿灯  4, 蓝灯
			case 0:
				break;
			case 1:
				colorWipe(Color(0, 0, 0), 5);
				break;
			case 2:
				colorWipe(Color(255, 0, 0), 5);
				break;
			case 3:
				colorWipe(Color(0, 255, 0), 5);
				break;
			case 4:
				colorWipe(Color(0, 0, 255), 5);
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
//		theaterChaseRainbow(50); //呼吸�??

	}
}

void SetPixelColor(uint16_t n, uint32_t color) {

	uint8_t r = (uint8_t) (color >> 16);
	uint8_t g = (uint8_t) (color >> 8);
	uint8_t b = (uint8_t) color;

	uint8_t tempBuffer[24];
	uint8_t i;

	for (i = 0; i < 8; i++) // GREEN
		tempBuffer[i] = ((g << i) & 0x80) ? WS2812_1 : WS2812_0;

	for (i = 0; i < 8; i++) // RED
		tempBuffer[8 + i] = ((r << i) & 0x80) ? WS2812_1 : WS2812_0;

	for (i = 0; i < 8; i++) // BLUE
		tempBuffer[16 + i] = ((b << i) & 0x80) ? WS2812_1 : WS2812_0;

	for (i = 0; i < 24; i++)
		ColorBuffer[RESET_SLOTS_BEGIN + n * 24 + i] = tempBuffer[i];

}

void PixelUpdate() {
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, LED_BUFFER_SIZE);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, uint8_t wait) {
	uint16_t i = 0;
	for (i = 0; i < LED_NUMBER; i++) {
		SetPixelColor(i, color);
		PixelUpdate();
		osDelay(wait);
	}
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
	return ((uint32_t) r << 16) | ((uint32_t) g << 8) | b;
}

uint32_t Wheel(uint8_t WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
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
void theaterChase(uint32_t c, uint8_t wait) {
	for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
		for (int q = 0; q < 3; q++) {
			for (uint16_t i = 0; i < LED_NUMBER; i = i + 3) {
				SetPixelColor(i + q, c); //turn every third pixel on
			}
			PixelUpdate();
			osDelay(wait);

			for (uint16_t i = 0; i < LED_NUMBER; i = i + 3) {
				SetPixelColor(i + q, 0); //turn every third pixel off
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
				SetPixelColor(i + q, 0); //turn every third pixel off
				PixelUpdate();
				osDelay(wait);
			}
		}
	}
}