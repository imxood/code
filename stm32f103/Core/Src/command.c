#include <stdio.h>
#include <command.h>
#include <ws2812b.h>
#include <stm32f1xx_ll_usart.h>
#include <stdlib.h>
#include <string.h>

// Receive commands from the host computer
volatile uint8_t CommandBuffer[CommandMaxLen];

volatile uint8_t CommandAcked = 0; // 已校验
volatile uint8_t AckData[32]; // 串口发送确认信息
volatile uint8_t AckDataLen[32]; // 串口发送的确认信息的长度

// Header + Instruction + Length + Parameters + CheckSum

// Header: 0xff 0xff
// Instruction: 0x01(write)
// Length: the number of bytes of instruction and parameter
// Parameters:
// CheckSum: ~(Instruction + Length + Parameters)

// example: 0xff 0xff 0x01 0x01 0x03 0xfa

CommandStatus CheckCommand(uint8_t* cmd, uint8_t cmdLen) {

	uint8_t checkSum = 0;
	CommandStatus status = CommandSuccess;

	if (cmd == NULL || cmdLen < 6) {
		status = CommandNullError;
	}

	else if (cmd[0] != 0xff && cmd[1] != 0xff) {
		status = CommandHeaderError;
	}

	else if (cmd[2] != 0x01) {
		status = CommandInstructionError;
	}

	if (!status) {

		for (int i = 0; i < cmd[3]; i++) {
			checkSum += cmd[4 + i];
		}
		checkSum = ~(checkSum + cmd[3] + cmd[2]);

		if (checkSum != cmd[4 + cmd[3]]) {
			status = CommandChecksumError;
		}

	}

	if (status) {
		printf("%d", status);
	}

	return status;

}

// example: 0xff 0xff 0x00 0x01 0x00 0xfe
void AckCommand(uint8_t* data, uint8_t dataLen) {

	uint8_t bufLen = 4 + dataLen;
	uint8_t* buf = (uint8_t *) malloc(bufLen);

	buf[0] = 0x02;
	buf[1] = 0x00;
	buf[2] = dataLen;

	uint8_t i;
	uint8_t checkSum = 0;

	memcpy(buf + 3, data, dataLen);

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
		AckData[0] = 0x01;
		AckDataLen = 1;
		CommandAcked = 0;
		return;
	}

	AckData[0] = 0x00;
	AckDataLen = 1;
	CommandAcked = 0;

	led_control(cmd + 4, cmd[3]);

}

