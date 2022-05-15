#pragma once
#include "Texture.h"
#include "RenderTargetGroup.h"

class Engine
{
public:

	void Init(WindowInfo _info);
	void Update();
	void Render();

private:
	void CreateRenderTargetGroups();
public:
	WindowInfo GetWinfo() { return mWinfo; }
	shared_ptr<RenderTargetGroup> GetRTGroup(RENDER_TARGET_GROUP_TYPE type) { return _rtGroups[static_cast<uint8>(type)]; }
	
private:
	WindowInfo mWinfo;
	array<shared_ptr<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> _rtGroups;
};

