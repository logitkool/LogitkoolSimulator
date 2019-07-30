#include "Code.h"

int Code::count;
Packet Code::Input(Packet packet)
{
	if (packet.count < 0)
	{
		return { TMode::Response, -1, dir, -1 };
	}

	if (is_recieved)
	{
		if (packet.mode == TMode::Echo)
		{
			return { TMode::Echo, packet.id, dir, packet.count - 1 };
		}
		else
		{
			return { packet.mode, packet.id, core_dir, packet.count - 1 };
		}
	}

	is_recieved = true;
	core_dir = DirUtil::Invert(packet.dir);

	return { TMode::Response, id, core_dir, packet.count - 1 };
}

void Code::SetDir(Direction _dir)
{
	dir = _dir;
}

Direction Code::GetDir() const
{
	return dir;
}

int Code::GetId() const
{
	return id;
}

void Code::SetId()
{
	if (id == -1)
	{
		id = count;
		count++;
	}
}

Code::Code()
{
	block = Block::None;
	dir = Direction::Down;
	is_recieved = false;

	id = -1;
}

Code::Code(const Code& code)
{
	block = code.block;
	core_dir = code.core_dir;
	dir = code.dir;
	id = code.id;
	is_recieved = code.is_recieved;
}


Code::~Code()
{
}