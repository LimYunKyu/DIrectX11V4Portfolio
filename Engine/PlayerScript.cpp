#include "pch.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "Animator.h"
#include "GameObject.h"

void PlayerScript::Update()
{


	_keyPress = false;

	if (!_isAttack)
	{
		TryMove();

		if (!_keyPress)
		{

			uint8 idx = 0;

			if (!_isBreathless)
			{
				if (_currentState == BLADER_ANIMSTATE::BATTLE_IDLE)
				{
					idx = static_cast<uint8>(BLADER_ANIMSTATE::WEAPON_IN);
					_currentState = BLADER_ANIMSTATE::WEAPON_IN;
				}
				else if (_currentState == BLADER_ANIMSTATE::WEAPON_IN)
				{
					idx = static_cast<uint8>(BLADER_ANIMSTATE::WEAPON_IN);
					if (CheckAnimFinish())
					{
						idx = static_cast<uint8>(BLADER_ANIMSTATE::IDLE);
						_currentState = BLADER_ANIMSTATE::IDLE;
					}
				}
				else
				{
					idx = static_cast<uint8>(BLADER_ANIMSTATE::IDLE);
					_currentState = BLADER_ANIMSTATE::IDLE;
				}

				

			}
			else
			{
				idx = static_cast<uint8>(BLADER_ANIMSTATE::BATTLE_IDLE);
				_currentState = BLADER_ANIMSTATE::BATTLE_IDLE;
			}
			

			PlayerAnimation(idx);

		}
	}
	
	TryAttack();




	
}

void PlayerScript::TryMove()
{

	Vec3 myPos = GetTransform()->GetLocalPosition();
	Vec3 myRot = GetTransform()->GetLocalRotation();

	auto MeshHolder = GetGameObject()->GetChilds()[1];

	Vec3 MeshHolderRot = MeshHolder->GetTransform()->GetLocalRotation();

	Vec3 Dir = {};


	if (KEY_INPUT->GetButton(KEY_TYPE::W))
	{


		Dir += GetTransform()->GetLook();// 오른손 좌표계라 Backward가 앞을 가리킴

		uint8 idx = 0;
		if (!_isBreathless)
		{
			idx = static_cast<uint8>(BLADER_ANIMSTATE::RUN);
			_currentState = BLADER_ANIMSTATE::RUN;
		}
		else
		{
			idx = static_cast<uint8>(BLADER_ANIMSTATE::BATTLE_RUN);
			_currentState = BLADER_ANIMSTATE::BATTLE_RUN;
		}
		
		MeshHolderRot = { 0,0,0 };
		if (KEY_INPUT->GetButton(KEY_TYPE::D))
		{
			MeshHolderRot = { 0,45,0 };
		}

		if (KEY_INPUT->GetButton(KEY_TYPE::A))
		{
			MeshHolderRot = { 0,-45,0 };
		}
		MeshHolder->GetTransform()->SetLocalRotation(MeshHolderRot);


		PlayerAnimation(idx);
		_keyPress = true;
	}

	if (KEY_INPUT->GetButton(KEY_TYPE::S))
	{



		Dir -= GetTransform()->GetLook();// 오른손 좌표계라 Backward가 앞을 가리킴

		uint8 idx = 0;
		if (!_isBreathless)
		{
			idx = static_cast<uint8>(BLADER_ANIMSTATE::RUN);
			_currentState = BLADER_ANIMSTATE::RUN;
		}
		else
		{
			idx = static_cast<uint8>(BLADER_ANIMSTATE::BATTLE_RUN);
			_currentState = BLADER_ANIMSTATE::BATTLE_RUN;
		}

		MeshHolderRot = { 0,180,0 };

		if (KEY_INPUT->GetButton(KEY_TYPE::D))
		{
			MeshHolderRot = { 0,225,0 };
		}

		if (KEY_INPUT->GetButton(KEY_TYPE::A))
		{
			MeshHolderRot = { 0,135,0 };
		}
		MeshHolder->GetTransform()->SetLocalRotation(MeshHolderRot);

		PlayerAnimation(idx);
		_keyPress = true;
	}

	if (KEY_INPUT->GetButton(KEY_TYPE::A))
	{
		GetTransform()->GetLook();

		Dir -= GetTransform()->GetRight();// 오른손 좌표계라 Backward가 앞을 가리킴

		uint8 idx = 0;
		if (!_isBreathless)
		{
			idx = static_cast<uint8>(BLADER_ANIMSTATE::RUN);
			_currentState = BLADER_ANIMSTATE::RUN;
		}
		else
		{
			idx = static_cast<uint8>(BLADER_ANIMSTATE::BATTLE_RUN);
			_currentState = BLADER_ANIMSTATE::BATTLE_RUN;
		}

		MeshHolderRot = { 0,-90,0 };
		MeshHolder->GetTransform()->SetLocalRotation(MeshHolderRot);
		PlayerAnimation(idx);
		_keyPress = true;
	}

	if (KEY_INPUT->GetButton(KEY_TYPE::D))
	{

		GetTransform()->GetLook();

		Dir += GetTransform()->GetRight();// 오른손 좌표계라 Backward가 앞을 가리킴

		uint8 idx = 0;
		if (!_isBreathless)
		{
			idx = static_cast<uint8>(BLADER_ANIMSTATE::RUN);
			_currentState = BLADER_ANIMSTATE::RUN;
		}
		else
		{
			idx = static_cast<uint8>(BLADER_ANIMSTATE::BATTLE_RUN);
			_currentState = BLADER_ANIMSTATE::BATTLE_RUN;
		}

		MeshHolderRot = { 0,90,0 };
		MeshHolder->GetTransform()->SetLocalRotation(MeshHolderRot);

		PlayerAnimation(idx);

		_keyPress = true;
	}

	Dir.Normalize();

	myPos += Dir * DELTA_TIME * _speed;
	GetTransform()->SetLocalPosition(myPos);
}

void PlayerScript::TryAttack()
{
	if (KEY_INPUT->GetButton(KEY_TYPE::LBUTTON))
	{

		//Attack
		_isAttack = true;
		_isAttackCheckStart = true;
		

		int idx = static_cast<uint8>(BLADER_ANIMSTATE::ATTACK_01) + _attackCount;

		PlayerAnimation(idx);


		_keyPress = true;
	}

	if (CheckAnimFinish() && _isAttack == true)
	{
		_isAttack = false;
		_attackCount++;
		_attackTime = 0.0f;
		_isBreathlessCheckStart = true;
		_breathlessTime = 0.0f;
	}

	if (_isAttackCheckStart)
	{
		
		_attackTime += DELTA_TIME;
		
		if (_attackTime >= _maxAttackTime)
		{
			_attackTime = 0;
			_attackCount = 0;
			_isAttack = false;
			
		}
	}

	if (_isBreathlessCheckStart)
	{
		_isBreathless = true;
		_breathlessTime += DELTA_TIME;

		if (_breathlessTime >= _maxBreathlessTime)
		{
			_isBreathlessCheckStart = false;
			_breathlessTime = 0.0f;
			_isBreathless = false;
		}

	}

	if (_attackCount >= 3)
		_attackCount = 0;
}

bool PlayerScript::CheckAnimFinish()
{

	auto MeshHolder = GetGameObject()->GetChilds()[1];
	auto MeshVec = MeshHolder->GetChilds();

	auto mesh = MeshVec[0];
	
		mesh->GetAnimator()->GetAnimFinish();
	


	return mesh->GetAnimator()->GetAnimFinish();;
}

void PlayerScript::PlayerAnimation(int idx)
{

	auto MeshHolder = GetGameObject()->GetChilds()[1];
	auto MeshVec = MeshHolder->GetChilds();

	for (auto mesh : MeshVec)
	{
		mesh->GetAnimator()->Play(idx);
	}

}
