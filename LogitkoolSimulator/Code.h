#pragma once
#include "Util.h"
#include <map>
#include <stdexcept>

class Code
{
public:
	// �u���b�N�̎��
	Block block;
	
	// ��M
	Packet Input(Packet packet);

	void SetDir(Direction _dir);

	Direction GetDir() const;

	int GetId() const;

	void SetId();

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
	Code(const Code& code);
	~Code();

	
};

