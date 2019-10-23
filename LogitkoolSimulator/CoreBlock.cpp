#include "CoreBlock.h"

void CoreBlock::Receive(Packet packet)
{

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
