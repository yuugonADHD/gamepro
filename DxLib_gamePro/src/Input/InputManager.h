#pragma once

#include <array>

/**
 * @brief キーボードとゲームパッド入力を管理するシングルトンクラス
 */
class InputManager
{
public:
    /**
     * @brief InputManagerのインスタンスを取得
     * @return InputManagerの唯一のインスタンス
     */
    static InputManager& GetInstance();

    /**
     * @brief 現在フレームの入力状態を更新
     * @return なし。
     */
    void Update();

    /**
     * @brief 指定キーがこのフレームで押されたか判定
     * @param keyCode DxLibのキーコード
     * @return 押された瞬間ならtrue、それ以外はfalse
     */
    bool IsKeyPressed(int keyCode) const;

    /**
     * @brief 指定キーが押し続けられているか判定
     * @param keyCode DxLibのキーコード
     * @return 押下中ならtrue、それ以外はfalse
     */
    bool IsKeyHeld(int keyCode) const;

    /**
     * @brief 指定キーがこのフレームで離されたか判定
     * @param keyCode DxLibのキーコード
     * @return 離された瞬間ならtrue、それ以外はfalse
     */
    bool IsKeyReleased(int keyCode) const;

    /**
     * @brief 現在のゲームパッド入力状態を取得
     * @return ゲームパッドのビットマスク状態
     */
    int GetPadInput() const;

    /**
     * @brief 指定ボタンがこのフレームで押されたか判定
     * @param buttonMask 判定したいボタンのビットマスク
     * @return 押された瞬間ならtrue、それ以外はfalse
     */
    bool IsPadButtonPressed(int buttonMask) const;

    /**
     * @brief 指定ボタンが押し続けられているか判定
     * @param buttonMask 判定したいボタンのビットマスク
     * @return 押下中ならtrue、それ以外はfalse
     */
    bool IsPadButtonHeld(int buttonMask) const;

    /**
     * @brief 指定ボタンがこのフレームで離されたか判定
     * @param buttonMask 判定したいボタンのビットマスク
     * @return 離された瞬間ならtrue、それ以外はfalse
     */
    bool IsPadButtonReleased(int buttonMask) const;

private:
    /**
     * @brief InputManagerの生成を禁止するためのコンストラクタ
     * @return なし。
     */
    InputManager();

    /**
     * @brief InputManagerのコピーを禁止
     * @param other コピー元オブジェクト
     * @return なし。
     */
    InputManager(const InputManager& other) = delete;

    /**
     * @brief InputManagerの代入を禁止
     * @param other 代入元オブジェクト
     * @return 代入結果の参照
     */
    InputManager& operator=(const InputManager& other) = delete;

    /** @brief 現在フレームのキー状態 */
    std::array<char, 256> currentKeys_{};

    /** @brief 前フレームのキー状態 */
    std::array<char, 256> previousKeys_{};

    /** @brief 現在フレームのゲームパッド状態 */
    int currentPadState_;

    /** @brief 前フレームのゲームパッド状態 */
    int previousPadState_;
};
