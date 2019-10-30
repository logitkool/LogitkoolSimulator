#include "Block.h"
#include "Singleton.h"
#include "ConnectionManager.h"

Role Block::GetRole() const
{
	return id.RoleType;
}

void Block::Block::SetDir(Direction _dir)
{
	dir = _dir;
}

bool Block::Connect(const std::shared_ptr<Block>& blk, Direction side)
{
	if (!canConnect(blk, side)) return false;

	ConnectionType conType = ConnectionType::Broadcast;

	// �ڑ��������H
	if (side == Direction::Down)
	{
		conType = ConnectionType::Forward;
	}

	singleton<ConnectionManager>::get_instance().Connect(id, conType, blk);

	return true;
}

Direction Block::Block::GetDir() const
{
	return dir;
}

bool Block::canConnect(const std::shared_ptr<Block>& blk, Direction side)
{
	// �R�A�̐ڑ������ȊO�Ƃ͌q����Ȃ�
	if (TypeUtil::IsSameType(Type::Core, blk->GetRole()))
	{
		return DirUtil::DirLookedFromBlock(blk->GetDir(), DirUtil::Invert(side)) == Direction::Down;
	}

	return true;
}

Block::Block(const BlockId& _id) : id(_id)
{
	dir = Direction::Down;
}

Block::Block(const Block& blk) : id(blk.id)
{
	dir = blk.dir;
}
