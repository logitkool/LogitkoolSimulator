
# include <Siv3D.hpp> // OpenSiv3D v0.3.2
#include "Util.h"
#include "Code.h"

void Main()
{
	const Font font(10);
	
	const int size = 15;
	const int cellSize = 35;
	constexpr Point offset(20, 40);

	const int paletteSize = 55;
	constexpr Point palletePos(600, 40);

	Array<Texture> blocks{ Texture(U"block/core.png"), Texture(U"block/move.png") };

	Grid<Code> cells(cellSize, cellSize);

	Code select = Code();
	Point corePos = Point();

	auto cellDraw = [blocks](const Code& code, const Rect & r)
	{
		if (code.block == Block::None)
		{
			return;
		}

		blocks[code.block]
			.resized(r.size)
			.rotated(code.GetDir() * 90_deg)
			.draw(r.pos);
	};
	
	while (System::Update())
	{
		for (auto y = 0; y < size; y++)
		{
			for (auto x = 0; x < size; x++)
			{
				const Rect rect = Rect(x * cellSize, y * cellSize, cellSize).movedBy(offset);

				auto& code = cells[y][x];

				if (code.block != Block::None)
				{
					cellDraw(code, rect);
				}

				// 格子の描画
				rect.drawFrame(1, Palette::Green);

				if (rect.mouseOver())
				{
					cellDraw(select, rect);
				}

				if (rect.leftClicked())
				{
					if (select.block == Block::Core 
						&& cells.any([](const Code & code) { return code.block == Block::Core; }))
					{
						// 一つ以上のコアブロックは追加できない
					}
					else
					{
						// フィールドにブロックを追加
						code = Code(select);
						code.SetId();
						
						if (select.block == Block::Core)
						{
							corePos = Point(x, y);
						}
					}
				}

				if (rect.rightClicked())
				{
					code.block = Block::None;
				}

				font(code.GetId()).draw(rect.pos, Palette::Purple);
			}
		}

		for (auto i = 0; i < blocks.size(); i++)
		{
			const Rect rect = Rect(paletteSize, i * paletteSize, paletteSize).movedBy(palletePos);

			rect(blocks[i]).draw();

			if (rect.leftClicked())
			{
				select.block = (Block)i;
			}
		}


		if (select.block != Block::None)
		{
			blocks[select.block]
				.rotated(select.GetDir() * 90_deg)
				.draw(600, 400);
		}

		if (Mouse::Wheel() > 0 || KeyLeft.down())
		{
			select.SetDir((Direction)((select.GetDir() + 1) % 4));
		}
		else if (Mouse::Wheel() < 0 || KeyRight.down())
		{
			select.SetDir((Direction)((select.GetDir() + 4 - 1) % 4));
		}


		if (KeySpace.down())
		{
			auto& core = cells[corePos];
			auto nextToCore = corePos + DirUtil::DirToPoint(core.GetDir());
			
			Packet corePkt = { TMode::Echo, core.GetId(), core.GetDir() };

			int step = 100;
			Array<int> idList;
			auto pos = nextToCore;
			auto pkt = cells[pos].Input(corePkt);
			while (pkt.id != -1)
			{
				pkt.Print();
				pos += DirUtil::DirToPoint(pkt.dir);

				if (cells[pos].block == Block::Core)
				{
					idList.push_back(pkt.id);
					pkt = corePkt;
					pos = nextToCore;
				}
				pkt = cells[pos].Input(pkt);
			}

			Print << idList;
		}

	}
}
