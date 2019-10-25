
#include <Siv3D.hpp> // OpenSiv3D v0.3.2

#include "Util.h"
#include "MoveBlock.h"
#include "CoreBlock.h"

void Main()
{
	const Font font(10);

	const int size = 15;
	const int cellSize = 35;
	constexpr Point offset(20, 40);

	const int paletteSize = 55;
	const int wrapCount = 5;
	constexpr Point palletePos(555, 100);

	JSONReader json(U"./block/textures.json");
	HashTable<Role, Texture> textureTable = TextureUtil::GetTextureTable(json, U"./block/");

	Grid<Optional<Block>> cells(cellSize, cellSize);

	BlockId select = { Role::None, 0x80, 0x01 };
	Direction selectedDir = Direction::Down;

	auto cellDraw = [textureTable](const Role& role, const Direction& dir, const Rect& r)
	{
		textureTable.at(role)
			.resized(r.size)
			.rotated(static_cast<int>(dir) * 90_deg)
			.draw(r.pos);
	};

	while (System::Update())
	{
		for (auto y = 0; y < size; y++)
		{
			for (auto x = 0; x < size; x++)
			{
				const Rect rect = Rect(x * cellSize, y * cellSize, cellSize).movedBy(offset);

				auto& blk = cells[y][x];

				if (blk.has_value())
				{
					cellDraw(blk->GetRole(), blk->GetDir(), rect);
				}

				// 格子の描画
				rect.drawFrame(1, Palette::Green);

				if (rect.mouseOver() && select.RoleType != Role::None)
				{
					cellDraw(select.RoleType, selectedDir, rect);
				}

				if (rect.leftClicked())
				{
					if (select.RoleType == Role::PureCore
						&& cells.any([](const Optional<Block>& blk) { return blk.has_value() && blk->GetRole() == Role::PureCore; }))
					{
						// 一つ以上のコアブロックは追加できない
					}
					else
					{
						// フィールドにブロックを追加
						if (select.RoleType != Role::None)
						{
							if (TypeUtil::IsSameType(Type::Core, select.RoleType))
							{
								// コアブロック
								blk = CoreBlock(select);
							}
							else if (TypeUtil::IsSameType(Type::If, select.RoleType) || TypeUtil::IsSameType(Type::For, select.RoleType))
							{
								// 分岐/繰り返しブロック
								// not implemented
							}
							else
							{
								// 動作ブロック
								blk = MoveBlock(select);
							}
							blk->SetDir(selectedDir);

							// increment id
							if (select.Uid_L == 0xFF)
							{
								select.Uid_H++;
								select.Uid_L = 0x00;
							}
							select.Uid_L++;

						}
					}
				}

				if (rect.rightClicked())
				{
					blk = none;
				}

				BlockId id = blk.has_value() ? blk->Id() : None;
				font(U"{:02X}\n{:02X}.{:02X}"_fmt(id.RoleId(), id.Uid_H, id.Uid_L)).draw(rect.pos, Palette::Purple);
			}
		}

		{
			// 全てのブロックの描画 (パレット)
			// コア
			{
				const Rect rect = Rect(paletteSize * 2, paletteSize).movedBy(555, 40);

				rect(textureTable[Role::PureCore]).draw();

				if (rect.leftClicked())
				{
					select.RoleType = Role::PureCore;
				}
			}

			// コア以外
			int dx = 0, dy = 0;
			for (const auto& [key, value] : textureTable)
			{
				if (key == Role::PureCore) continue;

				const Rect rect = Rect(dx * paletteSize, dy * paletteSize, paletteSize).movedBy(palletePos);

				rect(value).draw();

				if (rect.leftClicked())
				{
					select.RoleType = key;
				}

				dy++;
				if (dy >= wrapCount)
				{
					dx++;
					dy = 0;
				}
			}
		}


		if (select.RoleType != Role::None)
		{
			// 選択中のブロックを右下に描画
			textureTable[select.RoleType]
				.resized(select.RoleType == Role::PureCore ? 160 : 80, 80)
				.rotated(static_cast<int>(selectedDir) * 90_deg)
				.draw(555, 400);
		}

		if (Mouse::Wheel() > 0 || KeyLeft.down())
		{
			selectedDir = (Direction)((static_cast<int>(selectedDir) + 1) % 4);
		}
		else if (Mouse::Wheel() < 0 || KeyRight.down())
		{
			selectedDir = (Direction)((static_cast<int>(selectedDir) + 4 - 1) % 4);
		}


		if (KeySpace.down())
		{
			/*auto& core = cells[corePos];
			auto nextToCore = corePos + DirUtil::DirToPoint(core.GetDir());

			Packet corePkt = { TMode::Echo, core.GetId(), core.GetDir() };

			int step = 100;
			Array<int> idList;
			auto pos = nextToCore;
			auto pkt = cells[pos].Input(corePkt);
			while (pkt.id != -1)
			{
				pos += DirUtil::DirToPoint(pkt.dir);

				if (cells[pos].block == Block::Core)
				{
					idList.push_back(pkt.id);
					pkt = corePkt;
					pos = nextToCore;
				}

				pkt = cells[pos].Input(pkt);
			}

			Print << idList;*/
		}

	}
}
