#pragma once

#include <DxLib.h>

/**
 * @brief シャドウマップを管理するクラス
 *
 * 使用パターン:
 *   BeginDraw() → 影を落とすモデルの描画 → EndDraw()
 *   Apply()     → シーン全体の描画       → Clear()
 */
// 注意: シャドウマップはMV1DrawModel()で描画したMV1モデルにのみ有効
// DrawTriangle3D・DrawSphere3D 等のプリミティブ描画関数には非対応
// MV1モデルを使用するシーンでのみ利用すること
class ShadowMap
{
public:
    /**
     * @brief シャドウマップを生成
     * @return なし。
     */
    ShadowMap();

    /**
     * @brief シャドウマップを破棄
     * @return なし。
     */
    ~ShadowMap();

    /**
     * @brief シャドウマップを初期化。シーン開始時に1回呼び出す
     * @param width  シャドウマップの横解像度（大きいほど精度向上）
     * @param height シャドウマップの縦解像度
     * @return 成功したらtrue、失敗したらfalse
     */
    bool Init(int width = 1024, int height = 1024);

    /**
     * @brief シャドウマップを解放
     * @return なし。
     */
    void Finalize();

    /**
     * @brief 影を落とすライトの方向を設定
     * @param dir ライトの方向ベクトル
     * @return なし。
     */
    void SetLightDirection(VECTOR dir);

    /**
     * @brief シャドウマップが影響する範囲を設定
     * @param center 範囲の中心座標
     * @param range  半径
     * @return なし。
     */
    void SetArea(VECTOR center, float range);

    /**
     * @brief シャドウマップへの描画を開始
     * @return なし。
     */
    void BeginDraw();

    /**
     * @brief シャドウマップへの描画を終了
     * @return なし。
     */
    void EndDraw();

    /**
     * @brief シャドウマップをレンダリングに適用
     * @return なし。
     */
    void Apply();

    /**
     * @brief シャドウマップの適用を解除
     * @return なし。
     */
    void Clear();

private:
    /** @brief DxLibのシャドウマップハンドル。-1のときは未初期化 */
    int shadowMapHandle_;
};
