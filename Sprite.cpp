#include "Sprite.h"

#include <d3dx12.h>

using namespace DirectX;

Sprite* Sprite::Create(SpriteCommon* spriteCommon, UINT texNumber, DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	//�������m��
	Sprite* instance = new Sprite();
	//�C���X�^���X������
	instance->Initialize(spriteCommon, texNumber, anchorpoint, isFlipX, isFlipY);

	return instance;
}

void Sprite::Initialize(SpriteCommon* spriteCommon, UINT texNumber, DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;

	spriteCommon_ = spriteCommon;

	// �e�N�X�`���ԍ����R�s�[
	texNumber_ = texNumber;

	// �A���J�[�|�C���g���R�s�[
	anchorpoint_ = anchorpoint;

	// ���]�t���O���R�s�[
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	// ���_�f�[�^
	VertexPosUv vertices_[4];

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (spriteCommon_->GetTexBuff(texNumber_)) {
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetTexBuff(texNumber_)->GetDesc();

		// �X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		size_ = { (float)resDesc.Width, (float)resDesc.Height };
		texSize_ = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// ���_�o�b�t�@����
	result = spriteCommon_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices_)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff_));

	// ���_�o�b�t�@�f�[�^�]��
	TransferVertexBuffer();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices_);
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	// �萔�o�b�t�@�̐���
	result = spriteCommon_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // �F�w��iRGBA�j
	constMap->mat = spriteCommon->GetMatProjection();
	constBuff_->Unmap(0, nullptr);
}

void Sprite::TransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	// ���_�f�[�^
	VertexPosUv vertices_[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // ����
		{{}, {0.0f, 0.0f}}, // ����
		{{}, {1.0f, 1.0f}}, // �E��
		{{}, {1.0f, 0.0f}}, // �E��
	};

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint_.x) * size_.x;
	float right = (1.0f - anchorpoint_.x) * size_.x;
	float top = (0.0f - anchorpoint_.y) * size_.y;
	float bottom = (1.0f - anchorpoint_.y) * size_.y;

	if (isFlipX_)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY_)
	{// ���E����ւ�
		top = -top;
		bottom = -bottom;
	}

	vertices_[LB].pos = { left, bottom,  0.0f }; // ����
	vertices_[LT].pos = { left, top,     0.0f }; // ����
	vertices_[RB].pos = { right, bottom, 0.0f }; // �E��
	vertices_[RT].pos = { right, top,    0.0f }; // �E��

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (spriteCommon_->GetTexBuff(texNumber_)) {
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetTexBuff(texNumber_)->GetDesc();

		float tex_left = texLeftTop_.x / resDesc.Width;
		float tex_right = (texLeftTop_.x + texSize_.x) / resDesc.Width;
		float tex_top = texLeftTop_.y / resDesc.Height;
		float tex_bottom = (texLeftTop_.y + texSize_.y) / resDesc.Height;

		vertices_[LB].uv = { tex_left,   tex_bottom }; // ����
		vertices_[LT].uv = { tex_left,   tex_top }; // ����
		vertices_[RB].uv = { tex_right,  tex_bottom }; // �E��
		vertices_[RT].uv = { tex_right,  tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices_, sizeof(vertices_));
	vertBuff_->Unmap(0, nullptr);
}

void Sprite::Update()
{
	// ���[���h�s��̍X�V
	matWorld_ = XMMatrixIdentity();
	// Z����]
	matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));
	// ���s�ړ�
	matWorld_ *= XMMatrixTranslation(position_.x, position_.y, position_.z);

	// �萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld_ * spriteCommon_->GetMatProjection();
	constMap->color = color_;
	constBuff_->Unmap(0, nullptr);
}

void Sprite::Draw()
{
	if (isInvisible_) {
		return;
	}

	ID3D12GraphicsCommandList* commandList = spriteCommon_->GetCommandList();

	// ���_�o�b�t�@���Z�b�g
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	// ���[�g�p�����[�^�[0�Ԃɒ萔�o�b�t�@���Z�b�g
	commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	// ���[�g�p�����[�^�[1�ԂɃV�F�[�_���\�[�X�r���[���Z�b�g
	spriteCommon_->SetGraphicsRootDescriptorTable(1, texNumber_);

	// �|���S���̕`��i4���_�Ŏl�p�`�j
	commandList->DrawInstanced(4, 1, 0, 0);
}
