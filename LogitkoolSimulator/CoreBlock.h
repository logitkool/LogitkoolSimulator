#pragma once
#include "Packet.h"
#include "Block.h"
#include "Singleton.h"
#include "ConnectionManager.h"

class CoreBlock : public Block
{
public:
	void Receive(Packet packet);
	void Scan();
	bool Connect(const std::shared_ptr<Block>& blk, Direction side) override;

private:
	void sendToBlock(Packet packet);

	// ���O�ɑ��M�����p�P�b�g
	Packet prevPacket;

public:
	CoreBlock(const BlockId& _id);
	CoreBlock(const CoreBlock& code);
	~CoreBlock();

};
