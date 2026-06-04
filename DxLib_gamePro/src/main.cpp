#include "Common/Constants.h"
#include "Input/InputManager.h"
#include "Scene/SceneManager.h"

#include <DxLib.h>

/**
 * @brief ゲームのエントリーポイント
 * @return 正常終了時は0、初期化失敗時は-1
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    SetOutApplicationLogValidFlag(FALSE);
    ChangeWindowMode(TRUE);
    SetGraphMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, 32);
    SetMainWindowText(Constants::WINDOW_TITLE);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // 3D描画に必要なZバッファを有効化
    // これがないと奥行き順に正しく描画されない
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    // 3Dライティングを有効化
    SetUseLighting(TRUE);

    SetDrawScreen(DX_SCREEN_BACK);

    SceneManager sceneManager;
    sceneManager.ChangeScene(SceneID::Title);

    while (ProcessMessage() == 0)
    {
        // フレームの先頭で入力状態を更新
        InputManager::GetInstance().Update();

        // どのシーンからでもESCキーでゲームを終了
        if (InputManager::GetInstance().IsKeyPressed(KEY_INPUT_ESCAPE))
        {
            break;
        }

        // シーンのロジック更新
        sceneManager.Update();

        // 画面を消去してから現在シーンを描画
        ClearDrawScreen();
        sceneManager.Draw();

        ScreenFlip();

        // 簡易的なFPS制御でCPU使用率を抑制
        WaitTimer(1000 / Constants::TARGET_FPS);
    }

    DxLib_End();
    return 0;
}
