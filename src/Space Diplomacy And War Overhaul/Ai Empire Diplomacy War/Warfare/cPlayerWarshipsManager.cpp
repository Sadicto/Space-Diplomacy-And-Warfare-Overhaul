#include "stdafx.h"
#include "cPlayerWarshipsManager.h"

cPlayerWarshipsManager::cPlayerWarshipsManager()
{
}


cPlayerWarshipsManager::~cPlayerWarshipsManager()
{
}

// For internal use, do not modify.
int cPlayerWarshipsManager::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cPlayerWarshipsManager::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cPlayerWarshipsManager::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cPlayerWarshipsManager);
	return nullptr;
}
