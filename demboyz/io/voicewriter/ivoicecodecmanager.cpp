
#include "ivoicecodecmanager.h"
#include "steamvoicecodecmanager.h"
#include "celtcodecmanager.h"
#include "vaudioceltcodecmanager.h"

#include <string.h>

#define USE_VAUDIO_CELT 0 && _WIN32

IVoiceCodec::~IVoiceCodec()
{
}

IVoiceCodecManager::~IVoiceCodecManager()
{
}

IVoiceCodecManager* IVoiceCodecManager::Create(const char* codec)
{
	if (!strcmp(codec, "vaudio_celt"))
	{
#if USE_VAUDIO_CELT
		return CreateVAudioCeltCodecManager();
#else
		return CreateCeltCodecManager();
#endif
	}
	else if (!strcmp(codec, "steam"))
	{
		return CreateSteamCodecManager();
	}
	return nullptr;
}
