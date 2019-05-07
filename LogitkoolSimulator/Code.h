#pragma once
#include "Util.h"
#include <map>
#include <stdexcept>

class Code
{
public:
	// ブロックの種類
	Block block;
	
	// 送信モードか
	std::map<Direction, bool> trans_mode;

	// 受信
	void Input(Direction input_dir);

	void SetDir(Direction _dir);

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
	~Code();

	
};

