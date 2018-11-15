#pragma once

#include <stdint.h>

#define CommandMaxLen 256

typedef enum _CommandStatus{
	CommandSuccess, CommandNullError, CommandHeaderError, CommandInstructionError, CommandLengthError, CommandChecksumError
} CommandStatus;

void assign_task(uint8_t* cmd, uint8_t cmdLen);
