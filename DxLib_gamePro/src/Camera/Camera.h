#pragma once

#include <DxLib.h>

/**
 * @brief 3Dカメラを管理するクラス
 *
 * 固定カメラモードと追従カメラモードをサポート
 * 毎フレーム Update() を呼び出すことでカメラをDxLibに反映
 */
class Camera
{
public:
    /**
     * @brief カメラを生成。デフォルトは固定カメラモード
     * @return なし。
     */
    Camera();

    /**
     * @brief カメラの位置を設定（固定カメラモード用）
     * @param pos カメラのワールド座標
     * @return なし。
     */
    void SetPosition(VECTOR pos);

    /**
     * @brief カメラの注視点を設定（固定カメラモード用）
     * @param target 注視するワールド座標
     * @return なし。
     */
    void SetTarget(VECTOR target);

    /**
     * @brief 追従カメラモードでターゲットに追従
     * @param targetPos 追従対象のワールド座標
     * @param offset ターゲットからのカメラのオフセット
     * @return なし。
     */
    void Follow(VECTOR targetPos, VECTOR offset);

    /**
     * @brief 画角（垂直画角）をラジアンで設定
     * @param fov 画角（ラジアン）
     * @return なし。
     */
    void SetFov(float fov);

    /**
     * @brief ニアクリップとファークリップを設定
     * @param nearClip ニアクリップ距離
     * @param farClip ファークリップ距離
     * @return なし。
     */
    void SetClip(float nearClip, float farClip);

    /**
     * @brief カメラの状態をDxLibに反映。毎フレーム呼び出す
     * @return なし。
     */
    void Update();

private:
    /** @brief カメラのワールド座標 */
    VECTOR position_;

    /** @brief カメラの注視点のワールド座標 */
    VECTOR target_;

    /** @brief 画角（ラジアン） */
    float fov_;

    /** @brief ニアクリップ距離 */
    float nearClip_;

    /** @brief ファークリップ距離 */
    float farClip_;
};
