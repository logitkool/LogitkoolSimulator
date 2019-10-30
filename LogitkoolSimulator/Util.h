#pragma once
#include <Siv3D.hpp>
#include <map>

#include "Type.h"

class TypeUtil
{
public:
	static bool IsSameType(Type type, Role role)
	{
		return (static_cast<uint8_t>(role) & 0xF0) == static_cast<uint8_t>(type);
	}

	static int GetInterval(Role role)
	{
		switch (role)
		{
		case Role::MoveFront:
		case Role::MoveBack:          return 6000;

		case Role::TurnLeft:
		case Role::TurnRight:         return 8000;

		case Role::TurnLeft90:
		case Role::TurnRight90:       return 12000;

		case Role::ShakeLeftHand:
		case Role::ShakeRightHand:
		case Role::ShakeBothHands:    return 3000;

		case Role::ShakeLeftHead:
		case Role::ShakeRightHead:    return 4000;

		case Role::IfBrightness:
		case Role::IfObject:
		case Role::For1:
		case Role::For2:
		case Role::For3:
		case Role::For4:
		case Role::For5:              return 1000;

		default:                      return 0;
		}
	}

private:
	TypeUtil() {}

};

class DirUtil
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

	// ブロックから見た向きがグリッド平面上でどちら向きか
	static Direction DirLookedFromGrid(Direction baseDir, Direction side)
	{
		return static_cast<Direction>((static_cast<int>(baseDir) + static_cast<int>(side)) % NUM_DIRECTION);
	}

	// グリッド平面上で見た向きはブロックにとってはどちら向きか
	static Direction DirLookedFromBlock(Direction baseDir, Direction gridDir)
	{
		int dir = (NUM_DIRECTION - static_cast<int>(baseDir)) % NUM_DIRECTION;
		return DirLookedFromGrid(static_cast<Direction>(dir), gridDir);
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

private:
	DirUtil() {}

};

class TextureUtil
{
public:
	static HashTable<Role, Texture> GetTextureTable(const JSONValue& object, const String& basePath)
	{
		HashTable<Role, Texture> table;

		for (const auto& elm : object[U"paths"].arrayView())
		{
			for (const auto& item : elm.objectView())
			{
				uint8_t roleId = Parse<uint8_t>(item.name);
				Role role = static_cast<Role>(roleId);
				table.emplace(role, Texture(basePath + item.value.getString()));
			}
		}

		return table;
	}

private:
	TextureUtil() {}
};
