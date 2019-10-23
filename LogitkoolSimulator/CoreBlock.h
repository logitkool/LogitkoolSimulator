#pragma once
#include "Packet.h"
#include "Block.h"
#include "Singleton.h"
#include "ConnectionManager.h"

class CoreBlock : public Block
{
public:
	void Receive(Packet packet);

private:
	void sendToBlock(Packet packet);

	// 直前に送信したパケット
	Packet prevPacket;

public:
	CoreBlock(const BlockId& _id);
	CoreBlock(const CoreBlock& code);
	~CoreBlock();

};
