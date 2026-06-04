#pragma once

/**
 * @brief ゲーム全体で使用する定数をまとめた名前空間
 */
namespace Constants
{
    /** @brief 画面の横幅 */
    constexpr int SCREEN_WIDTH = 1280;

    /** @brief 画面の縦幅 */
    constexpr int SCREEN_HEIGHT = 720;

    /** @brief 目標フレームレート */
    constexpr int TARGET_FPS = 60;

    /** @brief ウィンドウタイトル */
    constexpr const char* WINDOW_TITLE = "TemplateDxLib";
}
