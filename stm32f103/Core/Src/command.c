#include <stdio.h>
//#include <stddef.h>
//#include <stdint.h>
#include <command.h>
#include <ws2812b.h>

// Receive commands from the host computer
volatile uint8_t CommandBuffer[CommandMaxLen];

// Header + Instruction + Length + Parameters + CheckSum

// Header: 0xff 0xff
// Instruction: 0x01(write)
// Length: the number of bytes of instruction and parameter
// Parameters:
// CheckSum: ~(Instruction + Length + Parameters)

static CommandStatus check_data(uint8_t* cmd, uint8_t cmdLen) {

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

void assign_task(uint8_t* cmd, uint8_t cmdLen) {

	CommandStatus status = check_data(cmd, cmdLen);

	if (status != CommandSuccess) {
		return;
	}

	led_control(cmd + 4, cmd[3]);
}

