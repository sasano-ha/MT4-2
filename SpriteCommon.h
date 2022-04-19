#pragma once

#include "PipelineSet.h"

#include <DirectXTex.h>

/// <summary>
/// �X�v���C�g���ʕ���
/// </summary>

class SpriteCommon
{
public:	//�v���W�F�N�g�S�̂Ŏg����

	// �e�N�X�`���̍ő喇��
	static const int kSpriteSRVCount = 512;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="commandList">�R�}���h���X�g</param>
	/// <param name="windows_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, int windows_width, int window_height);

	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();

	/// <summary>
	/// ���[�g�f�X�N���v�^�e�[�u���̐ݒ�
	/// </summary>
	/// <param name="rootParameterIndex">���[�g�p�����[�^�[�ԍ�</param>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="texNumber"></param>
	/// <param name="filename"></param>
	void LoadTexture(UINT texnumber, const wchar_t* filename);

	/// <summary>
	/// �e�N�X�`���擾
	/// </summary>
	/// <param name="texnumber">�e�N�X�`���ԍ�</param>
	/// <returns>�e�N�X�`�����\�[�X</returns>
	ID3D12Resource* GetTexBuff(int texNumber);


	ID3D12Device* GetDevice() { return device_; }

	const DirectX::XMMATRIX& GetMatProjection() { return matProjection_; }

	ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }

private:	//�錾�݂̂Ŏg����
	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet_;
	// �ˉe�s��
	DirectX::XMMATRIX matProjection_{};
	// �e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap_;
	// �e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j�̔z��
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_[kSpriteSRVCount];
	//�f�o�C�X�i�؂�Ă���j
	ID3D12Device* device_ = nullptr;
	//�R�}���h���X�g�i�؂�Ă���j
	ID3D12GraphicsCommandList* commandList_ = nullptr;

private:
	// �p�C�v���C������
	void CreateGraphicsPipeline();
};

