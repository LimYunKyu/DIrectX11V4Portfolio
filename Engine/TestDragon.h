#pragma once
#include "MonoBehaviour.h"

class TestDragon : public MonoBehaviour
{
public:
	virtual void Update() override;
	float		_speed = 100.f;
};

