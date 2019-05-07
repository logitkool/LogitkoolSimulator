#pragma once
#include "Util.h"
#include <map>
#include <stdexcept>

class Code
{
public:
	// �u���b�N�̎��
	Block block;
	
	// ���M���[�h��
	std::map<Direction, bool> trans_mode;

	// ��M
	void Input(Direction input_dir);

	void SetDir(Direction _dir);

private:
	// ���߂̎�M����
	Direction core_dir;

	// ���łɎ�M�ς݂�
	bool is_recieved;

	int id;

	// �����Ă������
	Direction dir;

	static int count;

public:
	Code();
	~Code();

	
};

