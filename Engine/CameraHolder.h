#pragma once
#include "MonoBehaviour.h"
class CameraHolder :
    public MonoBehaviour
{

public:
    void Update();

private:
    bool _recordStart = true;
    bool _directionSyncStart = false;
    float _saveMoveCharge = 0.f;
    Vec3 _saveRot = {};
    POINT _currentPos = {};
    POINT _prePos = {};
};

