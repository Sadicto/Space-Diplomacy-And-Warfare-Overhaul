#pragma once

#include <Spore\BasicIncludes.h>

#define cPlayerWarshipsManagerPtr intrusive_ptr<cPlayerWarshipsManager>

class cPlayerWarshipsManager 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("cPlayerWarshipsManager");
	
	cPlayerWarshipsManager();
	~cPlayerWarshipsManager();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
};
