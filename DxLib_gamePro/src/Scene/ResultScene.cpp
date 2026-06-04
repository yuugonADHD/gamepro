#include "ResultScene.h"

#include "../Input/InputManager.h"
#include "SceneManager.h"

#include <DxLib.h>

/**
 * @brief リザルトシーンを生成
 * @param sceneManager シーン遷移を担うシーンマネージャー
 * @return なし。
 */
ResultScene::ResultScene(SceneManager* sceneManager)
    : BaseScene(sceneManager)
{
}

/**
 * @brief リザルトシーンを破棄
 * @return なし。
 */
ResultScene::~ResultScene() = default;

/**
 * @brief リザルトシーンの初期化
 * @return なし。
 */
void ResultScene::Init()
{
    // TODO: スコア集計や結果の表示準備処理を追加
}

/**
 * @brief リザルトシーンの更新処理
 * @return なし。
 */
void ResultScene::Update()
{
    // Enterキーを押した瞬間にタイトル画面へ戻るサンプル
    if (InputManager::GetInstance().IsKeyPressed(KEY_INPUT_RETURN))
    {
        sceneManager_->ChangeScene(SceneID::Title);
    }

    // TODO: リトライ選択やランキング送信処理をここに追加
}

/**
 * @brief リザルトシーンの描画処理
 * @return なし。
 */
void ResultScene::Draw()
{
    DrawString(40, 40, "Result Scene", GetColor(255, 255, 255));
    DrawString(40, 80, "Press ENTER to Title", GetColor(255, 255, 0));
    DrawString(40, 120, "Press ESC to Exit", GetColor(255, 180, 180));
    // TODO: スコア表示やリトライボタンの描画処理をここに追加
}
