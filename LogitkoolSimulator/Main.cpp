
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

	Grid<Optional<std::shared_ptr<Block>>> cells(cellSize, cellSize);

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
				auto& blk = cells.at(y, x);
				const Rect rect = Rect(x * cellSize, y * cellSize, cellSize).movedBy(offset);

				if (blk.has_value())
				{
					cellDraw(blk.value()->GetRole(), blk.value()->GetDir(), rect);
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
						&& cells.any([](const Optional<std::shared_ptr<Block>>& blk) { return blk.has_value() && blk.value()->GetRole() == Role::PureCore; }))
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
								blk = (std::shared_ptr<Block>)std::make_shared<CoreBlock>(select);
							}
							else if (TypeUtil::IsSameType(Type::If, select.RoleType) || TypeUtil::IsSameType(Type::For, select.RoleType))
							{
								// 分岐/繰り返しブロック
								// not implemented
								System::ShowMessageBox(U"分岐/繰り返しブロックはまだ実装されてないよ", MessageBoxStyle::Error);
								continue;
							}
							else
							{
								// 動作ブロック
								blk = (std::shared_ptr<Block>)std::make_shared<MoveBlock>(select);
							}
							blk.value()->SetDir(selectedDir);

							// ConnectionManagerに接続を登録
							for (auto i : Iota(NUM_DIRECTION))
							{
								Direction dir = static_cast<Direction>(i);
								auto p = Point(x, y) + DirUtil::DirToPoint(dir);

								if (cells[p].has_value())
								{
									Direction sideFromBlk = DirUtil::DirLookedFromBlock(blk.value()->GetDir(), dir),
										sideFromTarget = DirUtil::DirLookedFromBlock(cells[p].value()->GetDir(), DirUtil::Invert(dir));

									// Print << U"blk[{}] -> tgt[{}]"_fmt(static_cast<int>(sideFromBlk), static_cast<int>(sideFromTarget));
									Print << U"blk: " << blk.value()->Connect(cells[p].value(), sideFromBlk);
									Print << U"tgt: " << cells[p].value()->Connect(blk.value(), sideFromTarget);
								}
							}

							// IDをインクリメント
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
					// ConnectionManagerから接続を削除
					for (auto i : Iota(NUM_DIRECTION))
					{
						Direction dir = static_cast<Direction>(i);
						auto p = Point(x, y) + DirUtil::DirToPoint(dir);

						if (cells[p].has_value())
						{
							singleton<ConnectionManager>::get_instance().DisconnectOne(blk.value()->Id(), cells[p].value()->Id());
							singleton<ConnectionManager>::get_instance().DisconnectOne(cells[p].value()->Id(), blk.value()->Id());
						}
					}

					blk = none;
				}

				BlockId id = blk.has_value() ? blk.value()->Id() : None;
				font(U"{:02X}\n{:02X}.{:02X}"_fmt(id.RoleId(), id.Uid_H, id.Uid_L)).draw(rect.pos, Palette::Purple);
			}
		}

		{
			// 全てのブロックの描画 (パレット)
			// コア
			{
				const Rect rect = Rect(paletteSize, paletteSize).movedBy(555, 40);

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
				.resized(80)
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


		if (KeySpace.down()
			&& cells.any([](const Optional<std::shared_ptr<Block>>& blk) { return blk.has_value() && blk.value()->GetRole() == Role::PureCore; }))
		{
			Point corePos;
			cells.each_index([&corePos](const Point& pos, const Optional<std::shared_ptr<Block>>& blk)
			{
				if (blk.has_value() && blk.value()->GetRole() == Role::PureCore)
				{
					corePos = pos;
				}
			});
			auto& core = (std::shared_ptr<CoreBlock>&)(cells[corePos].value());

			core->Scan();
		}

		if (KeyM.down())
		{
			singleton<ConnectionManager>::get_instance().Print();
		}

	}
}
