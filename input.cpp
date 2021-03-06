#include "input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


void Input::Initialize(WinApp* winApp)
{
    HRESULT result;

    //借りてきたWinAppのインスタンスを記録
    this->winApp = winApp;

    // DirectInputオブジェクトの生成
    result = DirectInput8Create(
        winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

    //キーボードデバイス生成
    result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

    //入力データ形式セット
    result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式

    //排他的制御レベルのセット
    result = devkeyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
    HRESULT result;

    //前回のキー入力を保存
    memcpy(keyPre, key, sizeof(key));

    //キーボード情報の取得開始
    result = devkeyboard->Acquire();

    //全キーの入力情報を取得する
    result = devkeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
    //指定キーを押していればtrueを返す
    if (key[keyNumber]) {
        return true;
    }
    //そうでなければfalseを返す
    return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
    if (keyPre[keyNumber] && key[keyNumber]) {
        return true;
    }
    return false;
}
