#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Material.h"
#include "InstancingBuffer.h"
#include "FBXLoader.h"
#include "StructuredBuffer.h"
#include "DXManager.h"
#include "DXManager.h"

Mesh::Mesh() : Object(OBJECT_TYPE::MESH)
{

}

Mesh::~Mesh()
{

}

void Mesh::Create(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

void Mesh::Render(uint32 instanceCount, uint32 idx)
{

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DEVICECONTEXT->IASetVertexBuffers(0, 1, &_vertexBuffer,&stride,&offset); // Slot: (0~15)
	DEVICECONTEXT->IASetIndexBuffer(_vecIndexInfo[idx].buffer,DXGI_FORMAT_R32_UINT,0);
	DEVICECONTEXT->DrawIndexedInstanced(_vecIndexInfo[idx].count, instanceCount, 0, 0, 0);
	
}

void Mesh::Render(shared_ptr<InstancingBuffer>& buffer, uint32 idx)
{
	ID3D11Buffer* bufferViews[] = { _vertexBuffer, buffer->GetBuffer() };

	UINT stride = sizeof(Vertex) + sizeof(InstancingParams);
	UINT offset = 0;
	DEVICECONTEXT->IASetVertexBuffers(0, 2, bufferViews, &stride, &offset);
	DEVICECONTEXT->IASetIndexBuffer(_vecIndexInfo[idx].buffer,DXGI_FORMAT_R32_UINT, 0);

	DEVICECONTEXT->DrawIndexedInstanced(_vecIndexInfo[idx].count, buffer->GetCount(), 0, 0, 0);
}

shared_ptr<Mesh> Mesh::CreateFromFBX(const FbxMeshInfo* meshInfo, FBXLoader& loader)
{
	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->CreateVertexBuffer(meshInfo->vertices);

	for (const vector<uint32>& buffer : meshInfo->indices)
	{
		if (buffer.empty())
		{
			// FBX ������ �̻��ϴ�. IndexBuffer�� ������ ���� ���ϱ� �ӽ� ó��
			vector<uint32> defaultBuffer{ 0 };
			mesh->CreateIndexBuffer(defaultBuffer);
		}
		else
		{
			mesh->CreateIndexBuffer(buffer);
		}
	}

	if (meshInfo->hasAnimation)
		mesh->CreateBonesAndAnimations(loader);

	return mesh;
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	_vertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);


	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * (buffer).size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &(buffer[0]);
	DEVICE->CreateBuffer(&vbd, &vinitData, &_vertexBuffer);



}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	_indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _indexCount * sizeof(uint32);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * buffer.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &(buffer[0]);

	DEVICE->CreateBuffer(&ibd, &iinitData, &_indexBuffer);





	IndexBufferInfo info =
	{
		_indexBuffer,
		DXGI_FORMAT_R32_UINT,
		_indexCount
	};

	_vecIndexInfo.push_back(info);
}


void Mesh::CreateBonesAndAnimations(class FBXLoader& loader)
{
#pragma region AnimClip
	uint32 frameCount = 0;
	vector<shared_ptr<FbxAnimClipInfo>>& animClips = loader.GetAnimClip();
	for (shared_ptr<FbxAnimClipInfo>& ac : animClips)
	{
		AnimClipInfo info = {};

		info.animName = ac->name;
		info.duration = ac->endTime.GetSecondDouble() - ac->startTime.GetSecondDouble();

		int32 startFrame = static_cast<int32>(ac->startTime.GetFrameCount(ac->mode));
		int32 endFrame = static_cast<int32>(ac->endTime.GetFrameCount(ac->mode));
		info.frameCount = endFrame - startFrame;

		info.keyFrames.resize(ac->keyFrames.size());

		const int32 boneCount = static_cast<int32>(ac->keyFrames.size());
		for (int32 b = 0; b < boneCount; b++)
		{
			auto& vec = ac->keyFrames[b];

			const int32 size = static_cast<int32>(vec.size());
			frameCount = max(frameCount, static_cast<uint32>(size));
			info.keyFrames[b].resize(size);

			for (int32 f = 0; f < size; f++)
			{
				FbxKeyFrameInfo& kf = vec[f];
				// FBX���� �Ľ��� ������� ä���ش�
				KeyFrameInfo& kfInfo = info.keyFrames[b][f];
				kfInfo.time = kf.time;
				kfInfo.frame = static_cast<int32>(size);
				kfInfo.scale.x = static_cast<float>(kf.matTransform.GetS().mData[0]);
				kfInfo.scale.y = static_cast<float>(kf.matTransform.GetS().mData[1]);
				kfInfo.scale.z = static_cast<float>(kf.matTransform.GetS().mData[2]);
				kfInfo.rotation.x = static_cast<float>(kf.matTransform.GetQ().mData[0]);
				kfInfo.rotation.y = static_cast<float>(kf.matTransform.GetQ().mData[1]);
				kfInfo.rotation.z = static_cast<float>(kf.matTransform.GetQ().mData[2]);
				kfInfo.rotation.w = static_cast<float>(kf.matTransform.GetQ().mData[3]);
				kfInfo.translate.x = static_cast<float>(kf.matTransform.GetT().mData[0]);
				kfInfo.translate.y = static_cast<float>(kf.matTransform.GetT().mData[1]);
				kfInfo.translate.z = static_cast<float>(kf.matTransform.GetT().mData[2]);
			}
		}

		_animClips.push_back(info);
	}
#pragma endregion

#pragma region Bones
	vector<shared_ptr<FbxBoneInfo>>& bones = loader.GetBones();
	for (shared_ptr<FbxBoneInfo>& bone : bones)
	{
		BoneInfo boneInfo = {};
		boneInfo.parentIdx = bone->parentIndex;
		boneInfo.matOffset = GetMatrix(bone->matOffset);
		boneInfo.boneName = bone->boneName;
		_bones.push_back(boneInfo);
	}
#pragma endregion

#pragma region SkinData
	if (IsAnimMesh())
	{
		// BoneOffet ���
		const int32 boneCount = static_cast<int32>(_bones.size());
		vector<Matrix> offsetVec(boneCount);
		for (size_t b = 0; b < boneCount; b++)
			offsetVec[b] = _bones[b].matOffset;

		// OffsetMatrix StructuredBuffer ����
		_offsetBuffer = make_shared<StructuredBuffer>();
		_offsetBuffer->Init(sizeof(Matrix), static_cast<uint32>(offsetVec.size()), offsetVec.data());

		const int32 animCount = static_cast<int32>(_animClips.size());
		for (int32 i = 0; i < animCount; i++)
		{
			AnimClipInfo& animClip = _animClips[i];

			// �ִϸ��̼� ������ ����
			vector<AnimFrameParams> frameParams;
			frameParams.resize(_bones.size() * animClip.frameCount);

			for (int32 b = 0; b < boneCount; b++)
			{
				int32 keyFrameCount = static_cast<int32>(animClip.keyFrames[b].size());

				if (keyFrameCount > 18)
					keyFrameCount = 18;
				for (int32 f = 0; f < keyFrameCount; f++)
				{
					int32 idx = static_cast<int32>(boneCount * f + b);

					if (idx > frameParams.size())
						int a = 5;

					frameParams[idx] = AnimFrameParams
					{
						Vec4(animClip.keyFrames[b][f].scale),
						animClip.keyFrames[b][f].rotation, // Quaternion
						Vec4(animClip.keyFrames[b][f].translate)
					};
				}
			}

			// StructuredBuffer ����
			_frameBuffer.push_back(make_shared<StructuredBuffer>());
			_frameBuffer.back()->Init(sizeof(AnimFrameParams), static_cast<uint32>(frameParams.size()), frameParams.data());
		}
	}
#pragma endregion
}

Matrix Mesh::GetMatrix(FbxAMatrix& matrix)
{
	Matrix mat;

	for (int32 y = 0; y < 4; ++y)
		for (int32 x = 0; x < 4; ++x)
			mat.m[y][x] = static_cast<float>(matrix.Get(y, x));

	return mat;
}