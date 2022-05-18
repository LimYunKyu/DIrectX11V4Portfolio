#include "pch.h"
#include "TestDragon.h"
#include "Input.h"
#include "Animator.h"
#include "Transform.h"
#include "Timer.h"
#include "SceneManager.h"

void TestDragon::Update()
{


	Vec3 _rotation = GetTransform()->GetLocalRotation();
	if (KEY_INPUT->GetButtonDown(KEY_TYPE::KEY_1))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex + 1) % count;
		GetAnimator()->Play(0);
	}

	if (KEY_INPUT->GetButtonDown(KEY_TYPE::KEY_2))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex - 1 + count) % count;
		GetAnimator()->Play(1);
	}

	if (KEY_INPUT->GetButtonDown(KEY_TYPE::KEY_3))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex - 1 + count) % count;
		GetAnimator()->Play(2);
	}
	if (KEY_INPUT->GetButtonDown(KEY_TYPE::KEY_4))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex - 1 + count) % count;
		GetAnimator()->Play(3);
	}
	if (KEY_INPUT->GetButtonDown(KEY_TYPE::KEY_5))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex - 1 + count) % count;
		GetAnimator()->Play(4);
	}
	if (KEY_INPUT->GetButtonDown(KEY_TYPE::KEY_6))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex - 1 + count) % count;
		GetAnimator()->Play(5);
	}

	Vec3 pos = GetTransform()->GetLocalPosition();

	if (KEY_INPUT->GetButton(KEY_TYPE::W))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (KEY_INPUT->GetButton(KEY_TYPE::S))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (KEY_INPUT->GetButton(KEY_TYPE::A))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (KEY_INPUT->GetButton(KEY_TYPE::D))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	/*if (KEY_INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_INPUT->GetButton(KEY_TYPE::Z))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= DELTA_TIME * 0.5f;

		if (rotation.y < -180.f)
			int a = 5;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (KEY_INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = KEY_INPUT->GetMousePos();
		GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
	}*/


	GetTransform()->SetLocalPosition(pos);
}