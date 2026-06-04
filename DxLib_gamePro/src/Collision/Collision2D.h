#pragma once

/**
 * @brief 2Dコライダー間の当たり判定を提供するユーティリティクラス
 *
 * すべてのメソッドはstatic。インスタンスを作成せずに使用可能
 */
class Collision2D
{
public:
    /**
     * @brief 正方形同士の当たり判定（縦横同じサイズ）
     * @param ax    矩形Aの左上X座標
     * @param ay    矩形Aの左上Y座標
     * @param aSize 矩形Aの一辺のサイズ
     * @param bx    矩形Bの左上X座標
     * @param by    矩形Bの左上Y座標
     * @param bSize 矩形Bの一辺のサイズ
     * @return 衝突していればtrue、していなければfalse
     */
    static bool SquareToSquare(float ax, float ay, int aSize,
                                float bx, float by, int bSize);

    /**
     * @brief 矩形同士の当たり判定（縦横異なるサイズ）
     * @param ax      矩形Aの左上X座標
     * @param ay      矩形Aの左上Y座標
     * @param aWidth  矩形Aの横幅
     * @param aHeight 矩形Aの縦幅
     * @param bx      矩形Bの左上X座標
     * @param by      矩形Bの左上Y座標
     * @param bWidth  矩形Bの横幅
     * @param bHeight 矩形Bの縦幅
     * @return 衝突していればtrue、していなければfalse
     */
    static bool RectToRect(float ax, float ay, int aWidth, int aHeight,
                            float bx, float by, int bWidth, int bHeight);

    /**
     * @brief 円同士の当たり判定
     * @param ax      円Aの中心X座標
     * @param ay      円Aの中心Y座標
     * @param aRadius 円Aの半径
     * @param bx      円Bの中心X座標
     * @param by      円Bの中心Y座標
     * @param bRadius 円Bの半径
     * @return 衝突していればtrue、していなければfalse
     */
    static bool CircleToCircle(float ax, float ay, float aRadius,
                                float bx, float by, float bRadius);
};
