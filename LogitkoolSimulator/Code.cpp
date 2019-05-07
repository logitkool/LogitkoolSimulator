#include "Code.h"

int Code::count;
void Code::Input(Direction input_dir)
{
	if (trans_mode[input_dir])
	{
		throw std::invalid_argument("送信モードのポートに受信されました");
	}


	if (!is_recieved)
	{
		is_recieved = true;

		core_dir = input_dir;

		trans_mode[core_dir] = true;
	}

}

void Code::SetDir(Direction _dir)
{
	if (id == -1)
	{
		id = count;
		count++;
	}

	dir = _dir;
}

Code::Code()
{
	block = Block::None;
	dir = Direction::Down;

	for (int i = 0; i < 4; i++)
	{
		trans_mode[(Direction)i] = false;
	}

	id = -1;
}


Code::~Code()
{
}