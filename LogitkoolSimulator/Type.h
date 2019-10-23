#pragma once
#include <cstdint>

enum class Direction : int
{
	Down = 0,
	Left,
	Up,
	Right
};

enum ConnectionType
{
	Forward,
	Broadcast,
	OtherPort
};

enum class Type : uint8_t
{
	Move = 0x10,
	Turn = 0x20,
	ShakeHand = 0x30,
	ShakeHead = 0x40,
	Core = 0x80,
	If = 0x90,
	For = 0xC0
};

enum class Role : uint8_t
{
	MoveFront = 0x11,
	MoveBack = 0x19,

	TurnLeft = 0x21,
	TurnRight = 0x29,
	TurnLeft90 = 0x22,
	TurnRight90 = 0x2A,

	ShakeLeftHand = 0x31,
	ShakeRightHand = 0x35,
	ShakeBothHands = 0x39,

	ShakeLeftHead = 0x41,
	ShakeRightHead = 0x49,

	PureCore = 0x81,

	IfBrightness = 0x91,
	IfObject = 0x92,

	For1 = 0xC1,
	For2 = 0xC2,
	For3 = 0xC3,
	For4 = 0xC4,
	For5 = 0xC5,

	None = 0xFF
};

enum class Mode : uint8_t
{
	PRODUCTION = 0x01,
	CONFIG = 0x11,
	DEBUG = 0x12
};

struct BlockId
{
	// 1byte (Max: 0xFF)
	Role RoleType;
	// Uid: 2byte (Max: 0xFFFF)
	uint8_t Uid_H;
	uint8_t Uid_L;

	uint8_t RoleId() const { return static_cast<uint8_t>(RoleType); }

	bool operator== (const BlockId& rhs) const
	{
		return (RoleType == rhs.RoleType && Uid_H == rhs.Uid_H && Uid_L == rhs.Uid_L);
	}

	bool operator!= (const BlockId& rhs) const
	{
		return !(RoleType == rhs.RoleType && Uid_H == rhs.Uid_H && Uid_L == rhs.Uid_L);
	}

	bool operator< (const BlockId& rhs) const
	{
		if (RoleId() < rhs.RoleId()) return true;
		if (RoleId() == rhs.RoleId())
		{
			if (Uid_H < rhs.Uid_H) return true;
			if (Uid_H == rhs.Uid_H) return Uid_L < rhs.Uid_L;
		}

		return false;
	}

	BlockId()
	{
		RoleType = Role::None;
		Uid_H = 0xFF;
		Uid_L = 0xFF;
	}

	BlockId(Role role, uint8_t id_h, uint8_t id_l)
		: RoleType(role), Uid_H(id_h), Uid_L(id_l) {}

	BlockId(uint8_t role_id, uint8_t id_h, uint8_t id_l)
		: RoleType(static_cast<Role>(role_id)), Uid_H(id_h), Uid_L(id_l) {}
};

inline const BlockId None = { Role::None, 0xFF, 0xFF };
