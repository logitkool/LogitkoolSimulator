#pragma once
#include "Packet.h"
#include "Block.h"
#include "Singleton.h"
#include "ConnectionManager.h"

const int MAX_ID = 4;

class MoveBlock : public Block
{
public:
	void Receive(Packet packet) override;

	void SetMode(const Mode& _mode)
	{
		mode = _mode;
	}

private:
	void sentToAllPorts(Packet packet);
	void sendBroadcast(Packet packet);
	void sendConPort(Packet packet);

	// �p�P�b�g���������Ă��ǂ����ǂ��� (config���[�h������false)
	bool canProcess()
	{
		return mode == Mode::PRODUCTION || mode == Mode::DEBUG;
	}

	// ���łɎ�M�ς݂�
	bool is_recieved;

	// �אڃu���b�N���
	BlockId known_ids[MAX_ID];

	// ���݂̎��s���[�h
	Mode mode;

	// ���O�ɑ��M�����p�P�b�g
	Packet prevPacket;

public:
	MoveBlock(const BlockId& _id);
	MoveBlock(const MoveBlock& blk);
	~MoveBlock();
	
};

