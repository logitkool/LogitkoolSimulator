#pragma once
#include "Type.h"
#include "Util.h"
#include "Packet.h"

class Block
{
public:
	virtual void Receive(Packet pkt) {}
	virtual bool Connect(const std::shared_ptr<Block>& blk, Direction side);

	bool GetLed()
	{
		return ledState;
	}

	const BlockId& Id() const
	{
		return id;
	}

	Direction GetDir() const;
	Role GetRole() const;
	void SetDir(Direction _dir);

protected:
	BlockId id;
	Direction dir;
	bool ledState = false;

	virtual bool canConnect(const std::shared_ptr<Block>& blk, Direction side);

public:
	Block(const BlockId& _id);
	Block(const Block& blk);

};
