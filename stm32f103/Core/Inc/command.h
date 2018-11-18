#pragma once

#include <stdint.h>

#define CommandMaxLen 256

typedef enum _CommandStatus{
	CommandSuccess, CommandNullError, CommandHeaderError, CommandInstructionError, CommandLengthError, CommandChecksumError
} CommandStatus;

CommandStatus CheckCommand(uint8_t* cmd, uint8_t cmdLen);

void AckCommand(uint8_t* data, uint8_t dataLen);

void AssignTask(uint8_t* cmd, uint8_t cmdLen);
