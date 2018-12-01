#include <stdio.h>
#include <command.h>
#include <ws2812b.h>
#include <stm32f1xx_ll_usart.h>
#include <stdlib.h>
#include <string.h>

// Receive commands from the host computer
volatile uint8_t CommandBuffer[CommandMaxLen];

extern uint8_t CommandAcked; // 已确认
volatile uint8_t AckData[32]; // 串口发送确认信息
volatile uint8_t AckDataLen; // 串口发送的确认信息的长度

// 取出int类型的4个字节的指定位
// BIT(200, 2) --> (200 >> (2-1)) & 0xff
#define BIT(number, bit) ((char)((number >> (bit-1)) & 0xff))

// 0x02, rate, delay, count, [rgb], CheckSum
// CheckSum: ~(Instruction + Length + Parameters)
CommandStatus CheckCommand(uint8_t* cmd, uint8_t cmdLen) {

	uint8_t checkSum = 0;
	CommandStatus status = CommandSuccess;

	if (cmd == NULL || cmdLen < 12) {
		status = CommandNullError;
	}

	else if (cmd[0] != CommandHeader) {
		status = CommandHeaderError;
	}

	if (!status) {
		for (int i = 1; i < cmdLen - 1; i++) {
			checkSum += cmd[i];
		}
		checkSum = ~checkSum;

		if (checkSum != cmd[cmdLen - 1]) {
			status = CommandChecksumError;
		}
	}
	return status;
}

// example: 0xff 0xff 0x00 0x01 0x00 0xfe
void SendAckData() {

	uint8_t bufLen = 4 + AckDataLen;
	uint8_t* buf = (uint8_t *) malloc(bufLen);

	buf[0] = CommandHeader;
	buf[1] = 0x00;
	buf[2] = AckDataLen;

	uint8_t i;
	uint8_t checkSum = 0;

	memcpy(buf + 3, AckData, AckDataLen);

	for (i = 1; i < bufLen - 1; i++) {
		checkSum += buf[i];
	}

	checkSum = ~checkSum;

	buf[bufLen - 1] = checkSum;

	for (i = 0; i < bufLen; i++) {
		LL_USART_TransmitData8(USART1, buf[i]);
	}

	free(buf);

	CommandAcked = 1;
}

void AssignTask(uint8_t* cmd, uint8_t cmdLen) {
	uint8_t buf[1] = { 0x00 };
	CommandStatus status = CheckCommand(cmd, cmdLen);

	if (status != CommandSuccess) {
		buf[0] = 0x01;
		UpdateAckData(buf, 1);
		return;
	}

	UpdateAckData(buf, 1);

	led_control(cmd + 1, 10 + cmd[9] * 3);
}

void UpdateAckData(uint8_t* ackData, uint8_t ackDataLen) {
	memcpy(AckData, ackData, ackDataLen);
	AckDataLen = ackDataLen;
	CommandAcked = 0;
}

