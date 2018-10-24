#include "stm32f1xx_ll_usart.h"

// 把printf数据发送至串口
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE {
	USART1->DR = (uint8_t) ch;
	while ((USART1->SR & 0X40) == 0)
		;
	return ch;
}

//#pragma import(__use_no_semihosting)
////标准库需要的支持函数
//struct __FILE {
//	int handle;
//
//};
//
//FILE __stdout;
////定义_sys_exit()以避免使用半主机模式
//_sys_exit(int x) {
//	x = x;
//}
////重定义fputc函数
//int fputc(int ch, FILE *f) {
//	while ((USART1->SR & 0X40) == 0)
//		; //循环发�??,直到发�?�完�????????
//	USART1->DR = (uint8_t) ch;
//	return ch;
//}

int _write(int file, char *ptr, int len) {
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++) {
		__io_putchar(*ptr++);
	}
	return len;
}
