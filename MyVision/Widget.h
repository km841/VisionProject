#pragma once
#include "pch.h"
class Widget
{
public:
	Widget(const Vec2& _Size)
		: MSize(_Size)
	{ }
	virtual ~Widget() = default;

public:
	virtual void Initialize() { }
	virtual void Update() = 0;

public:
	void SetSize(Vec2 _Size) { MSize = _Size; }
	const Vec2& GetSize() { return MSize; }

protected:
	Vec2 MSize;
	std::string MName;
};

