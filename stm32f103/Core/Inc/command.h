#pragma once

#define CommandMaxLen 256

typedef enum _CommandStatus{
	CommandSuccess, CommandNullError, CommandHeaderError, CommandInstructionError, CommandLengthError, CommandChecksumError
} CommandStatus;

