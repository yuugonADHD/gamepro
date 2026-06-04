#include "InputManager.h"

#include <DxLib.h>

/**
 * @brief InputManagerの生成を禁止するためのコンストラクタ
 * @return なし。
 */
InputManager::InputManager()
    : currentPadState_(0)
    , previousPadState_(0)
{
}

/**
 * @brief InputManagerのインスタンスを取得
 * @return InputManagerの唯一のインスタンス
 */
InputManager& InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

/**
 * @brief 現在フレームの入力状態を更新
 * @return なし。
 */
void InputManager::Update()
{
    // 前フレーム状態を保存して押し上がり・離し判定に使用
    previousKeys_ = currentKeys_;
    previousPadState_ = currentPadState_;

    GetHitKeyStateAll(currentKeys_.data());
    currentPadState_ = GetJoypadInputState(DX_INPUT_PAD1);
}

/**
 * @brief 指定キーがこのフレームで押されたか判定
 * @param keyCode DxLibのキーコード
 * @return 押された瞬間ならtrue、それ以外はfalse
 */
bool InputManager::IsKeyPressed(const int keyCode) const
{
    return currentKeys_[keyCode] != 0 && previousKeys_[keyCode] == 0;
}

/**
 * @brief 指定キーが押し続けられているか判定
 * @param keyCode DxLibのキーコード
 * @return 押下中ならtrue、それ以外はfalse
 */
bool InputManager::IsKeyHeld(const int keyCode) const
{
    return currentKeys_[keyCode] != 0;
}

/**
 * @brief 指定キーがこのフレームで離されたか判定
 * @param keyCode DxLibのキーコード
 * @return 離された瞬間ならtrue、それ以外はfalse
 */
bool InputManager::IsKeyReleased(const int keyCode) const
{
    return currentKeys_[keyCode] == 0 && previousKeys_[keyCode] != 0;
}

/**
 * @brief 現在のゲームパッド入力状態を取得
 * @return ゲームパッドのビットマスク状態
 */
int InputManager::GetPadInput() const
{
    return currentPadState_;
}

/**
 * @brief 指定ボタンがこのフレームで押されたか判定
 * @param buttonMask 判定したいボタンのビットマスク
 * @return 押された瞬間ならtrue、それ以外はfalse
 */
bool InputManager::IsPadButtonPressed(const int buttonMask) const
{
    return (currentPadState_ & buttonMask) != 0 && (previousPadState_ & buttonMask) == 0;
}

/**
 * @brief 指定ボタンが押し続けられているか判定
 * @param buttonMask 判定したいボタンのビットマスク
 * @return 押下中ならtrue、それ以外はfalse
 */
bool InputManager::IsPadButtonHeld(const int buttonMask) const
{
    return (currentPadState_ & buttonMask) != 0;
}

/**
 * @brief 指定ボタンがこのフレームで離されたか判定
 * @param buttonMask 判定したいボタンのビットマスク
 * @return 離された瞬間ならtrue、それ以外はfalse
 */
bool InputManager::IsPadButtonReleased(const int buttonMask) const
{
    return (currentPadState_ & buttonMask) == 0 && (previousPadState_ & buttonMask) != 0;
}
