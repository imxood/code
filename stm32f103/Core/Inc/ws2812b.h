#ifndef __WS2812B_H
#define	__WS2812B_H

#include "cmsis_os.h"

#define WS2812_FREQ					(800000)
#define TIMER_CLOCK_FREQ			(72000000)
#define TIMER_PERIOD				(TIMER_CLOCK_FREQ / WS2812_FREQ)

#define LED_NUMBER					(40)
#define LED_DATA_SIZE				(LED_NUMBER * 24)
#define RESET_SLOTS_BEGIN			(50)
#define RESET_SLOTS_END				(50)
#define WS2812_LAST_SLOT			(1)
#define LED_BUFFER_SIZE				(RESET_SLOTS_BEGIN + LED_DATA_SIZE + WS2812_LAST_SLOT + RESET_SLOTS_END)

// 用于生成WS2812的脉冲
#define WS2812_0					(TIMER_PERIOD / 3)
#define WS2812_1					(TIMER_PERIOD * 2 / 3)
#define WS2812_RESET				(0)

typedef struct _Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} Color;

// ws2812 模块的外设使能初始化
void ws2812_init();

// 任务开始执行
void run();

// 更新
void WS2812_Update();

/*************************************************************
 * 函 数 名 : SetLedColor
 * 函数描述 : 设置某一盏LED颜色
 * 输入参数 : index  需要点亮的LED序号（第一盏、第二盏.....）
 * 输出参数 : none
 * 返 回 值 : none
 *************************************************************/

void SetPixelColor(uint16_t n, Color color);

// Fill the dots one after the other with a color
void colorWipe(Color color, uint8_t wait);

void PixelUpdate();

//uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

// 设置所有灯的颜色
void allColor(Color color, uint8_t wait);

Color Wheel(uint8_t WheelPos);

//彩虹
void rainbow(uint8_t wait);

//Theatre-style crawling lights.呼吸灯
void theaterChase(Color color, uint8_t wait);

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait);

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait);

void ledControlRequest();

// 白色渐变
void white_gradient(uint8_t wait);

Color ToColor(uint8_t r, uint8_t g, uint8_t b);

Color ToColorA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

// 把带有透明度的RGB转化成单纯的RGB
// colora: 需要有透明度的rgb颜色
// color: 转化后的rgb颜色
Color ColorA2Color(Color colora);

void RGB2HSL(Color color, double *h, double *l, double *s);

void HSL2RGB(double h, double l, double s, Color* color);

double HSL2RGBvalue(double n1, double n2, double hue);

#endif /* __WS2812B_H */
