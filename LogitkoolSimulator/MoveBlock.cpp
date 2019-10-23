#include "MoveBlock.h"

void MoveBlock::Receive(Packet packet)
{
	switch (packet.command)
	{
	case Command::ASK:
		if (!canProcess()) break;
		{
			unsigned int idx = 0;
			while (idx < MAX_ID && known_ids[idx].Uid_H != 0xFF && known_ids[idx].Uid_H != 0xFF) idx++;
			if (idx == MAX_ID) break;

			bool isKnown = false;
			for (unsigned int i = 0; i < idx; i++)
			{
				if (known_ids[i] == packet.src)
				{
					isKnown = true;
				}
			}

			if (isKnown)
			{
				if (packet == prevPacket) break;
				Packet pkt;
				pkt.command = Command::ASK;
				pkt.src = id;
				sendConPort(pkt);
			}
			else
			{
				known_ids[idx] = packet.src;
				Packet pkt;
				pkt.command = Command::RET;
				pkt.src = packet.src;
				pkt.dst = id;
				sentToAllPorts(pkt);
			}

			is_recieved = true;
		}
		break;

	case Command::RET:
	case Command::SWC:
		if (!canProcess()) break;
		{
			if (!is_recieved) break;
			if (packet == prevPacket) break;
			// isTerminal = false;
			sendBroadcast(packet);
		}
		break;

	case Command::CFG:
	{
		// ブロックを設定モードにする
		mode = Mode::CONFIG;
	}
	break;

	case Command::APL:
	{
		// 設定の内容を適応・保存する
		// config.apply(); // [important] EEPROM is disabled on simulator
	}
	break;

	case Command::SET:
	{
		// ブロックの設定を変更する
		// [important] EEPROM is disabled on simulator
		/*switch (data[1])
		{
			case static_cast<int>(Config::SettingType::MODE) :
				if (size < 3) break;
				config.changeMode(static_cast<Config::Mode>(data[2]));
				break;

				case static_cast<int>(Config::SettingType::ID) :
					if (size < 5) break;
					config.setId(data[2], data[3], data[4]);
					break;

				default:
					break;
		}*/
	}
	break;

	case Command::TXD:
		if (!canProcess()) break;
		{
			if (!is_recieved) break;
			if (packet == prevPacket) break;
			if (packet.dst == id)
			{
				switch (static_cast<Data>(packet.body[0]))
				{
				case Data::LED:
					ledState = (packet.body[1] == 0x01);
					break;

				default:
					break;
				}
			}
			else
			{
				sendConPort(packet);
			}
		}
		break;

	case Command::RST:
		if (!canProcess()) break;
		{
			if (packet == prevPacket) break;

			is_recieved = false;
			// isTerminal = true;
			prevPacket = Packet();

			for (unsigned int i = 0; i < MAX_ID; i++)
			{
				known_ids[i] = None;
			}

			/*for (int i = 0; i < 3; i++)
			{
				comm.sendToConPort(Command::RST);
			}*/
			Packet pkt;
			pkt.command = Command::RST;
			sendConPort(pkt);

		}
		break;

	default:
		break;

	}
}

void MoveBlock::sentToAllPorts(Packet packet)
{
	singleton<ConnectionManager>::get_instance().Send(id, ConnectionType::Forward, packet);
	singleton<ConnectionManager>::get_instance().Send(id, ConnectionType::Broadcast, packet);
}

void MoveBlock::sendBroadcast(Packet packet)
{
	singleton<ConnectionManager>::get_instance().Send(id, ConnectionType::Broadcast, packet);
}

void MoveBlock::sendConPort(Packet packet)
{
	singleton<ConnectionManager>::get_instance().Send(id, ConnectionType::Forward, packet);
}

MoveBlock::MoveBlock(const BlockId& _id) : Block(_id)
{
	is_recieved = false;
	mode = Mode::PRODUCTION;
}

MoveBlock::MoveBlock(const MoveBlock& blk) : Block(blk)
{
	is_recieved = blk.is_recieved;

}

MoveBlock::~MoveBlock()
{
}
