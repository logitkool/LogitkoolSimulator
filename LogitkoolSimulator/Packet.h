#pragma once

#include <Siv3D.hpp>

#include "commands.h"
#include "Type.h"

struct Packet
{
	Command command;

	BlockId src = None;
	BlockId dst = None;

	Array<uint8_t> body;

	bool operator== (const Packet& pkt)
	{
		bool ret = true;

		ret &= command == pkt.command;
		ret &= src == pkt.src;
		ret &= dst == pkt.dst;
		ret &= body == pkt.body;

		return ret;
	}

};
