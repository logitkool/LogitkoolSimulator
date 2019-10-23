#include "Block.h"

Role Block::GetRole() const
{
	return id.RoleType;
}

void Block::Block::SetDir(Direction _dir)
{
	dir = _dir;
}

Direction Block::Block::GetDir() const
{
	return dir;
}

Block::Block(const BlockId& _id) : id(_id)
{
	dir = Direction::Down;
}

Block::Block(const Block& blk) : id(blk.id)
{
	dir = blk.dir;
}
