#include "Sprite.h"

#include <DxLib.h>

/**
 * @brief 画像ファイルを読み込んでスプライトを生成
 * @param filePath 読み込む画像ファイルのパス
 * @return なし。
 */
Sprite::Sprite(const std::string& filePath)
    : graphHandle_(LoadGraph(filePath.c_str()))
{
}

/**
 * @brief スプライトを破棄
 * @return なし。
 */
Sprite::~Sprite()
{
    if (graphHandle_ >= 0)
    {
        DeleteGraph(graphHandle_);
    }
}

/**
 * @brief 指定座標にスプライトを描画
 * @param x 描画先のX座標
 * @param y 描画先のY座標
 * @return なし。
 */
void Sprite::Draw(const int x, const int y) const
{
    if (graphHandle_ >= 0)
    {
        DrawGraph(x, y, graphHandle_, TRUE);
    }
}

/**
 * @brief サイズ指定でスプライトを描画
 * @param x 描画先のX座標
 * @param y 描画先のY座標
 * @param width 描画幅
 * @param height 描画高さ
 * @return なし。
 */
void Sprite::Draw(const int x, const int y, const int width, const int height) const
{
    if (graphHandle_ >= 0)
    {
        DrawExtendGraph(x, y, x + width, y + height, graphHandle_, TRUE);
    }
}
