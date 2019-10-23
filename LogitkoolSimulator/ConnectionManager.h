#pragma once

#include <map>

#include "Block.h"

class ConnectionManager
{
public:
	ConnectionManager() {}

	void Connect(const BlockId& id, const ConnectionType& type, std::shared_ptr<Block> block)
	{
		auto key = std::make_pair(id, type);
		if (!connections.contains(key))
		{
			connections.emplace(key, Array<std::shared_ptr<Block>>());
		}

		connections.at(key).push_back(block);
	}

	void Disconnect(const BlockId& id, const ConnectionType& type, const BlockId& targetId)
	{
		auto key = std::make_pair(id, type);

		if (!connections.contains(key)) return;

		connections.at(key).remove_if([&](const std::shared_ptr<Block>& blk)
			{
				return blk->Id() == targetId;
			});
	}

	void Send(const BlockId& ownId, const ConnectionType& type, Packet packet)
	{
		auto key = std::make_pair(ownId, type);
		if (connections.contains(key))
		{
			// connection found.
			for (const auto& blk : connections.at(key))
			{
				blk->Receive(packet);
			}
		}
	}

private:
	std::map<std::pair<BlockId, ConnectionType>, Array<std::shared_ptr<Block>>> connections;

};
