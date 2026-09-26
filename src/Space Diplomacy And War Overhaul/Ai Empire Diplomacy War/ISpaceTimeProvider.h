#pragma once

#include <Spore\BasicIncludes.h>

#define ISpaceTimeProviderPtr intrusive_ptr<ISpaceTimeProvider>

class ISpaceTimeProvider 
{
public:
	static const uint32_t TYPE = id("SpaceDiplomacyWarfareOverhaul::ISpaceTimeProvider");
	
	virtual int AddRef() = 0;
	virtual int Release() = 0;
	virtual uint32_t GetCurrentSpaceTime() = 0;
};
