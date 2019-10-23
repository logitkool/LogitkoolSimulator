#pragma once
#include <cstdint>

enum class Command : uint8_t
{
	// 0x0- : config
	RST = 0x01,
	CFG = 0x81,
	APL = 0x82,
	SET = 0x91,
	// 0xA- : block scan
	ASK = 0xA0,
	RET = 0xA1,
	SWC = 0xA2,
	// 0xD- : data transfer
	DAT = 0xD0,
	TXD = 0xD1
};

enum class Data : uint8_t
{
	// data types
	LED = 0x01
};
