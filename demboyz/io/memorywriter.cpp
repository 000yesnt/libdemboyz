#include "idemowriter.h"
#include "demofile/demojson.h"
#include "demofile/demotypes.h"
#include "base/jsonfile.h"
#include "demmessages/demhandlers.h"
#include "netmessages/nethandlers.h"
#include "game/sourcecontext.h"
#include <cstdio>
#include <cassert>
#include <vector>

// TODO: Give the structs below better names
struct NetPacketWithContext
{
    NetPacket packet;
    SourceGameContext context;
};

struct CommandPacketAndTrailingBits 
{
    CommandPacket commandPacket;
    NetPacketWithContext netPacket;
    PacketTrailingBits trailingBits;
};

class MemoryWriter : public IDemoWriter {
public:
	MemoryWriter();

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(const CommandPacket& packet) override final;
    virtual void EndCommandPacket(const PacketTrailingBits& trailingBits) override final;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) override final;

    std::vector<CommandPacketAndTrailingBits> m_packets;
    demoheader_t m_header;

private:
    bool m_writingNetPackets;
    CommandPacket m_currentCommandPacket;
    NetPacket m_currentNetPacket;
    SourceGameContext m_currentGameContext;
};

void MemoryWriter::StartWriting(demoheader_t& header)
{
    m_header = header;
}

void MemoryWriter::EndWriting()
{
    // there's nothing we can do
}

void MemoryWriter::StartCommandPacket(const CommandPacket& packet) 
{
    m_currentCommandPacket = packet;
}

void MemoryWriter::EndCommandPacket(const PacketTrailingBits& trailingBits)
{
    CommandPacketAndTrailingBits pats = CommandPacketAndTrailingBits();
    pats.commandPacket = m_currentCommandPacket;
    if (m_writingNetPackets) {
        NetPacketWithContext npwc = NetPacketWithContext();
        npwc.packet = m_currentNetPacket;
        npwc.context = m_currentGameContext;

        pats.netPacket = npwc;
        m_writingNetPackets = false;
    }
    pats.trailingBits = trailingBits;
    m_packets.push_back(pats);
}

void MemoryWriter::WriteNetPacket(NetPacket& packet, SourceGameContext& context) 
{
    m_writingNetPackets = true;
    m_currentNetPacket = packet;
    m_currentGameContext = context;
}