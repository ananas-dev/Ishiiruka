// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <string>
#include <unordered_map>
#include <deque>

#include "SlippiLib/SlippiGame.h"
#include "Common/CommonTypes.h"
#include "Common/FileUtil.h"
#include "Core/HW/EXI_Device.h"

// Acts
class CEXISlippi : public IEXIDevice
{
public:
	CEXISlippi();
  virtual ~CEXISlippi();

	void ImmWrite(u32 data, u32 size) override;
	u32 ImmRead(u32 size) override;

	bool IsPresent() const override;

private:
	enum {
		CMD_UNKNOWN = 0x0,
		CMD_RECEIVE_COMMANDS = 0x35,
		CMD_RECEIVE_GAME_INFO = 0x36,
		CMD_RECEIVE_FRAME_UPDATE = 0x38,
		CMD_RECEIVE_GAME_END = 0x39,
		CMD_PREPARE_REPLAY = 0x75,
		CMD_READ_FRAME = 0x76,
		CMD_GET_LOCATION = 0x77
	};

	std::unordered_map<u8, u32> payloadSizes = {
		// The actual size of this command will be sent in one byte
		// after the command is received. The other receive command IDs
		// and sizes will be received immediately following
		{ CMD_RECEIVE_COMMANDS, 1},

		// The following are all commands used to play back a replay and
		// have fixed sizes
		{ CMD_PREPARE_REPLAY, 0 },
		{ CMD_READ_FRAME, 5 },
		{ CMD_GET_LOCATION, 5 }
	};

	// .slp File creation stuff
	u32 writtenByteCount = 0;
	void configureCommands(u8* payload, u8 length);
	void writeToFile(u8* payload, u32 length, std::string fileOption);
	void createNewFile();
	void closeFile();
	std::string generateFileName();

	File::IOFile m_file;
	u32 m_payload_loc = 0;
	u8 m_payload_type = CMD_UNKNOWN;
	std::vector<u8> m_payload;

	// replay playback stuff
	void loadFile(std::string path);
	void prepareGameInfo();
	void prepareFrameData(int32_t frameIndex, uint8_t port);
	void prepareLocationData(int32_t frameIndex, uint8_t port);

	std::deque<u32> m_read_queue;
	Slippi::SlippiGame* m_current_game = nullptr;

protected:
	void TransferByte(u8& byte) override;
};