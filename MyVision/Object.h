#pragma once
#include "pch.h"
class Object
{
public:
	Object(OBJECT_TYPE _EObjectType);
	virtual ~Object();
	virtual void Destroy() { }

public:
	void SetName(const std::wstring& _Name) { MName = _Name; }
	const std::wstring& GetName() { return MName; }
	OBJECT_TYPE GetType() { return EObjectType; }
	UINT32 GetID() { return MID; }

public:
	friend class Resources;
	virtual void Load(const std::wstring& _path) { }
	virtual void Save(const std::wstring& _path) { }

protected:
	std::wstring MName;
	OBJECT_TYPE EObjectType;
	UINT32 MID = 0;
};

