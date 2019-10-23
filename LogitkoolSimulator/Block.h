#pragma once
#include "Type.h"
#include "Util.h"
#include "Packet.h"

class Block
{
public:
	virtual void Receive(Packet pkt) {}

	bool getLed()
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

public:
	Block(const BlockId& _id);
	Block(const Block& blk);

};
