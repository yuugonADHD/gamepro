#pragma once

#include <DxLib.h>

/**
 * @brief シーン全体に影響する平行光源を管理するクラス
 */
class Light
{
public:
    /**
     * @brief ライトを生成
     * @return なし。
     */
    Light();

    /**
     * @brief ライトをデフォルト値で初期化。シーン開始時に1回呼び出す
     * @return なし。
     */
    void Init();

    /**
     * @brief 光の当たる方向を設定
     * @param dir 方向ベクトル（正規化推奨）
     * @return なし。
     */
    void SetDirection(VECTOR dir);

    /**
     * @brief 拡散光の色を設定
     * @param color 拡散光の色
     * @return なし。
     */
    void SetDiffuse(COLOR_F color);

    /**
     * @brief 環境光の色を設定
     * @param color 環境光の色
     * @return なし。
     */
    void SetAmbient(COLOR_F color);

    /**
     * @brief 鏡面反射光の色を設定
     * @param color 鏡面反射光の色
     * @return なし。
     */
    void SetSpecular(COLOR_F color);
};
