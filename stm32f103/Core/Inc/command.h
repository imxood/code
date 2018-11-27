#pragma once

#include <stdint.h>

#define CommandMaxLen 256
#define CommandHeader	0x02

typedef enum _CommandStatus{
	CommandSuccess, CommandNullError, CommandHeaderError, CommandInstructionError, CommandLengthError, CommandChecksumError
} CommandStatus;

typedef enum _CommmandType{
	CommandAck, CommandWrite, CommandRead
} CommmandType;


CommandStatus CheckCommand(uint8_t* cmd, uint8_t cmdLen);

void UpdateAckData(uint8_t* ackData, uint8_t ackDataLen);
void SendAckData();

void AssignTask(uint8_t* cmd, uint8_t cmdLen);

