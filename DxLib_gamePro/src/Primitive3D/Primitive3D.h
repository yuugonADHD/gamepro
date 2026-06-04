#pragma once

#include <DxLib.h>

/**
 * @brief 3D形状を描画するユーティリティクラス（デバッグ表示などに有用）
 *
 * すべてのメソッドはstatic。インスタンスを生成せずに使用可能
 */
class Primitive3D
{
public:
    /**
     * @brief 球を描画
     * @param center 球の中心座標
     * @param radius 球の半径
     * @param color  描画色
     * @param fill   trueのとき塗りつぶし、falseのときワイヤーフレーム
     * @return なし。
     */
    static void DrawSphere(VECTOR center, float radius, COLOR_U8 color,
                            bool fill = true, bool lighting = false);

    /**
     * @brief 軸並行境界ボックス（AABB）を描画
     * @param min  ボックスの最小座標
     * @param max  ボックスの最大座標
     * @param color 描画色
     * @param fill  trueのとき塗りつぶし、falseのときワイヤーフレーム
     * @return なし。
     */
    static void DrawBox(VECTOR min, VECTOR max, COLOR_U8 color,
                        bool fill = true, bool lighting = false);

    /**
     * @brief 線分を描画
     * @param start 始点の座標
     * @param end   終点の座標
     * @param color 描画色
     * @param width 線の太さ
     * @return なし。
     */
    static void DrawLine(VECTOR start, VECTOR end, COLOR_U8 color,
                         float width = 1.0f, bool lighting = false);

    /**
     * @brief カプセルを描画
     * @param start  カプセルの下端の中心座標
     * @param end    カプセルの上端の中心座標
     * @param radius カプセルの半径
     * @param color  描画色
     * @param fill   trueのとき塗りつぶし、falseのときワイヤーフレーム
     * @return なし。
     */
    static void DrawCapsule(VECTOR start, VECTOR end, float radius, COLOR_U8 color,
                             bool fill = true, bool lighting = false);

private:
    // インスタンス化禁止
    Primitive3D() = delete;
};
