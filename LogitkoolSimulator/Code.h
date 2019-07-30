#pragma once
#include "Util.h"
#include <map>
#include <stdexcept>

class Code
{
public:
	// ブロックの種類
	Block block;
	
	// 受信
	Packet Input(Packet packet);

	void SetDir(Direction _dir);

	Direction GetDir() const;

	int GetId() const;

	void SetId();

private:
	// 初めの受信方向
	Direction core_dir;

	// すでに受信済みか
	bool is_recieved;

	int id;

	// 向いている方向
	Direction dir;

	static int count;

public:
	Code();
	Code(const Code& code);
	~Code();

	
};

