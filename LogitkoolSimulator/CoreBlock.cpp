#include "CoreBlock.h"

void CoreBlock::Receive(Packet packet)
{
	Print << U"[CoreBlock] incomming packet.";
}

void CoreBlock::Scan()
{
	Print << U"[CoreBlock] pressed button.";

	Packet pkt;
	pkt.command = Command::ASK;
	pkt.src = id;
	sendToBlock(pkt);

}

bool CoreBlock::Connect(const std::shared_ptr<Block>& blk, Direction side)
{
	if (side == Direction::Down)
	{
		singleton<ConnectionManager>::get_instance().Connect(id, ConnectionType::Forward, blk);
		return true;
	}

	return false;
}

void CoreBlock::sendToBlock(Packet packet)
{
	singleton<ConnectionManager>::get_instance().Send(id, ConnectionType::Forward, packet);
}

CoreBlock::CoreBlock(const BlockId& _id) : Block(_id)
{
}

CoreBlock::CoreBlock(const CoreBlock& blk) : Block(blk.id)
{
}

CoreBlock::~CoreBlock() {}
