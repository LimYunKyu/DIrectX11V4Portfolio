#pragma once

#include "EnginePch.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine\\debug\\Engine.lib")
#else
#pragma comment(lib, "Engine\\release\\Engine.lib")
#endif