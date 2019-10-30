#include "ConnectionManager.h"

void ConnectionManager::Connect(const BlockId& id, const ConnectionType& type, const std::shared_ptr<Block>& block)
{
	auto key = std::make_pair(id, type);
	if (!connections.contains(key))
	{
		connections.emplace(key, Array<std::shared_ptr<Block>>());
	}

	connections.at(key).push_back(block);
}

void ConnectionManager::DisconnectOne(const BlockId& id, const BlockId& targetId)
{
	for(auto i : Iota(NUM_CONTYPE))
	{
		auto key = std::make_pair(id, static_cast<ConnectionType>(i));

		if (!connections.contains(key)) continue;

		connections.at(key).remove_if([&](const std::shared_ptr<Block>& blk)
		{
			return blk->Id() == targetId;
		});

	}
}

void ConnectionManager::Send(const BlockId& ownId, const ConnectionType& type, Packet packet)
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

void ConnectionManager::Print()
{
	int count = 0;
	for (const auto& p : connections)
	{
		count += p.second.count();
	}

	s3d::Print << U"[ConnectionManager] count: " << count;

}

