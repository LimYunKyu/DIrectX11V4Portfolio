#include "pch.h"
#include "CameraHolder.h"

#include "Transform.h"
#include "Input.h"
#include "Timer.h"

void CameraHolder::Update()
{

	Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 _rotation = GetTransform()->GetLocalRotation();

	if (_rotation.x <= 17)
	{
		_rotation.x = 17;
		GetTransform()->SetLocalRotation(_rotation);
	}


	if (KEY_INPUT->GetButton(KEY_TYPE::WHEEL))
	{

		if (_recordStart)
		{
			_saveRot = _rotation;
		}

		_recordStart = false;

		GetCursorPos(&_currentPos);

		float moveX = _currentPos.x - _prePos.x;
		float moveY = _currentPos.y - _prePos.y;

		_rotation.y -= moveX * DELTA_TIME * 100.f;
		_rotation.x -= moveY * DELTA_TIME * 100.f;

		if (_rotation.x >= 90)
		{
			_rotation.x = 90;
		}

		if (_rotation.x <= 17)
		{
			_rotation.x = 17;
		}



		_prePos = _currentPos;
		_directionSyncStart = true;
		GetTransform()->SetLocalRotation(_rotation);
	
	}




	if (KEY_INPUT->GetButton(KEY_TYPE::W) ||
		KEY_INPUT->GetButton(KEY_TYPE::A) ||
		KEY_INPUT->GetButton(KEY_TYPE::S) ||
		KEY_INPUT->GetButton(KEY_TYPE::D))
	{

		if (_directionSyncStart)
		{														//�ٷ� ī�޶� ȸ�� �������� ������ �ٽ� ���� �־�� �Ѵ�.


			_saveMoveCharge = (_rotation.y - _saveRot.y); //����Ű�� ������ ���� ���� Y�� ȸ������ ���

			auto parentTransform = GetTransform()->GetParent().lock()->GetTransform();
			auto parentRot = parentTransform->GetLocalRotation();
			parentRot.y += _saveMoveCharge;
			_rotation.y -= _saveMoveCharge;

			parentTransform->SetLocalRotation(parentRot);
			GetTransform()->SetLocalRotation(_rotation);


			_directionSyncStart = false;
			_recordStart = true;
		}

	}



	GetCursorPos(&_prePos);

}
