#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"

//DirectX���
class DirectXCommon
{

public:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	//�����o�֐�
	//������
	void Initialize(WinApp* win);
	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();
	//�f�o�C�X�̎擾
	ID3D12Device* GetDev() { return dev.Get(); }
	//�R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }


private:	//�����o�ϐ�
	//WindowsAPI
	WinApp* winApp = nullptr;

	// ���\�̂Q���ɂ���
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	// �t�F���X�̐���
	UINT64 fenceVal = 0;

	// Direct3D�֘A
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12Fence> fence;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

private:	//�����o�֐�
	//�f�o�C�X�̏�����
	void InitializeDevice();

	//�R�}���h�֘A�̏�����
	void InitializeCommand();

	//�X���b�v�`�F�[���̏�����
	void InitializeSwapchain();

	//�����_�[�^�[�Q�b�g�̏�����
	void InitializeRenderTargetView();

	//�[�x�o�b�t�@�̏�����
	void InitializeDepthBuffer();

	//�t�F���X�̏�����
	void InitializeFence();
};