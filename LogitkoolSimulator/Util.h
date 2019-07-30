#pragma once
#include <Siv3D.hpp>

#define STR(var) (#var)

enum Block
{
	Core,
	Move,
	None
};

enum Direction
{
	Down,
	Left,
	Up,
	Right
};

enum TMode
{
	Echo, Response
};

static class DirUtil
{
public:
	static Point DirToPoint(Direction dir)
	{
		switch (dir)
		{
		case Direction::Down:
			return Point::Down();
		case Direction::Up:
			return Point::Up();
		case Direction::Left:
			return Point::Left();
		case Direction::Right:
			return Point::Right();
		}
	}


	static Direction Invert(Direction dir)
	{
		switch (dir)
		{
		case Direction::Down:
			return Direction::Up;
		case Direction::Up:
			return Direction::Down;
		case Direction::Left:
			return Direction::Right;
		case Direction::Right:
			return Direction::Left;
		}
	}

};

struct Packet
{
	TMode mode;
	int id;
	Direction dir;
	int count = 100;

	void Print()
	{
		s3d::Print 
			<< (mode ? U"Echo" : U"Trans")
			<< U" (x, y): " << (DirUtil::DirToPoint(dir))
			<< U" id: " << id;
	}
};
