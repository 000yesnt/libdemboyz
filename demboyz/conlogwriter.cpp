
#include "idemowriter.h"
#include "netmessages/nethandlers.h"
#include "demotypes.h"
#include <cstdio>

class ConLogWriter: public IDemoWriter
{
public:
    ConLogWriter(FILE* outputFp);

    virtual void StartWriting(demoheader_t& header) override final;
    virtual void EndWriting() override final;

    virtual void StartCommandPacket(CommandPacket& packet) override final;
    virtual void EndCommandPacket() override final;

    virtual void WriteNetPacket(NetPacket& packet, SourceGameContext& context) override final;

private:
    FILE* m_outputFp;
};

IDemoWriter* IDemoWriter::CreateConLogWriter(void* outputFp)
{
    return new ConLogWriter(reinterpret_cast<FILE*>(outputFp));
}

ConLogWriter::ConLogWriter(FILE* outputFp):
    m_outputFp(outputFp)
{
}

void ConLogWriter::StartWriting(demoheader_t& header)
{
    //std::ostringstream ss;
    //ss << "Playing demo from " <<  << ".\n";
    //fputs(ss.str().c_str(), m_outputFp);
    //std::clog << ss.str() << std::endl;
    fflush(stdout);
    setvbuf(stdout, NULL, _IOFBF, 1024*64);
}

void ConLogWriter::EndWriting()
{
    fflush(stdout);
}

void ConLogWriter::StartCommandPacket(CommandPacket& packet)
{
}

void ConLogWriter::EndCommandPacket()
{
}

void ConLogWriter::WriteNetPacket(NetPacket& packet, SourceGameContext& context)
{
    std::ostringstream ss;
    NetHandlers::NetMsg_ToString(packet.type, ss, packet.data);
    if (ss.tellp() > 0)
    {
        ss << "\n";
        fputs(ss.str().c_str(), m_outputFp);
        fputs(ss.str().c_str(), stdout);
    }
}
