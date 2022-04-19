#pragma once
#include <Windows.h>

//WindowsAPI
class WinApp
{
public:	//�萔
	//�E�B���h�E����
	static const int windows_width = 1280;
	//�E�B���h�E�c��
	static const int windows_height = 720;
public:	//�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:	//�����o�֐�
	//������
	void Initialize();
	//�X�V
	void Update();
	//�I��
	void Finalize();
	//���b�Z�[�W�̏���
	bool ProcessMessage();
	//getter
	HWND GetHwnd() { return hwnd; }
	//getter
	HINSTANCE GetHInstance() { return w.hInstance; }
private:
	//�E�B���h�E�n���h��
	HWND hwnd = nullptr;
	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
};