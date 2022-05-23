#pragma once
#include "MonoBehaviour.h"

enum class BLADER_ANIMSTATE {

    BATTLE_IDLE,
    IDLE,
    WEAPON_IN,
    BATTLE_RUN,
    RUN,
    ATTACK_01,
    ATTACK_02,
    ATTACK_03,
    DAMAGE_01,
    DAMAGE_02,

};

class PlayerScript :
    public MonoBehaviour
{

public:
    void Update();
    void TryMove();
    void TryAttack();
    bool CheckAnimFinish();
    void PlayerAnimation(int idx);

private:
    bool    _keyPress = false;
    bool    _isAttack = false;
    float   _speed = 400.f;
    int     _attackCount = 0;
    float   _attackTime = 0.0f;
    float   _maxAttackTime = 3.0f;
    bool    _isAttackCheckStart = false;
    float   _breathlessTime = 0.0f;
    float   _maxBreathlessTime = 6.0f;
    bool    _isBreathless = false;
    bool    _isBreathlessCheckStart = false;


    BLADER_ANIMSTATE _currentState;
};

