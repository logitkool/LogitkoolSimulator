#pragma once

#include <map>

#include "Block.h"

class ConnectionManager
{
public:
	ConnectionManager() {}

	void Connect(const BlockId& id, const ConnectionType& type, const std::shared_ptr<Block>& block);

	void DisconnectOne(const BlockId& id, const BlockId& targetId);

	void Send(const BlockId& ownId, const ConnectionType& type, Packet packet);

	void Print();

private:
	std::map<std::pair<BlockId, ConnectionType>, Array<std::shared_ptr<Block>>> connections;

};
