#pragma once

#include <string>

/**
 * @brief 2D画像の読み込みと描画を簡単に扱うスプライトクラス
 */
class Sprite
{
public:
    /**
     * @brief 画像ファイルを読み込んでスプライトを生成
     * @param filePath 読み込む画像ファイルのパス
     * @return なし。
     */
    explicit Sprite(const std::string& filePath);

    /**
     * @brief スプライトを破棄
     * @return なし。
     */
    ~Sprite();

    /**
     * @brief 指定座標にスプライトを描画
     * @param x 描画先のX座標
     * @param y 描画先のY座標
     * @return なし。
     */
    void Draw(int x, int y) const;

    /**
     * @brief サイズ指定でスプライトを描画
     * @param x 描画先のX座標
     * @param y 描画先のY座標
     * @param width 描画幅
     * @param height 描画高さ
     * @return なし。
     */
    void Draw(int x, int y, int width, int height) const;

private:
    /** @brief DxLibのグラフィックハンドル */
    int graphHandle_;
};
