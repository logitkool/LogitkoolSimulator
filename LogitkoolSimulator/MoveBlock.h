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

	// パケットを処理しても良いかどうか (configモード中等はfalse)
	bool canProcess()
	{
		return mode == Mode::PRODUCTION || mode == Mode::DEBUG;
	}

	// すでに受信済みか
	bool is_recieved;

	// 隣接ブロック情報
	BlockId known_ids[MAX_ID];

	// 現在の実行モード
	Mode mode;

	// 直前に送信したパケット
	Packet prevPacket;

public:
	MoveBlock(const BlockId& _id);
	MoveBlock(const MoveBlock& blk);
	~MoveBlock();
	
};

