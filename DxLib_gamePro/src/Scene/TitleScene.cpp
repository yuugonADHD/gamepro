#include "TitleScene.h"

#include "../Input/InputManager.h"
#include "SceneManager.h"

#include <DxLib.h>

/**
 * @brief タイトルシーンを生成
 * @param sceneManager シーン遷移を担うシーンマネージャー
 * @return なし。
 */
TitleScene::TitleScene(SceneManager* sceneManager)
    : BaseScene(sceneManager)
{
}

/**
 * @brief タイトルシーンを破棄
 * @return なし。
 */
TitleScene::~TitleScene() = default;

/**
 * @brief タイトルシーンの初期化
 * @return なし。
 */
void TitleScene::Init()
{
    // TODO: タイトルロゴの画像読み込みやBGM再生開始処理を追加
}

/**
 * @brief タイトルシーンの更新処理
 * @return なし。
 */
void TitleScene::Update()
{
    // Enterキーを押した瞬間にゲーム本編へ遷移するサンプル
    if (InputManager::GetInstance().IsKeyPressed(KEY_INPUT_RETURN))
    {
        sceneManager_->ChangeScene(SceneID::Game2D);
    }

    // TODO: メニュー選択や難易度選択処理をここに追加
}

/**
 * @brief タイトルシーンの描画処理
 * @return なし。
 */
void TitleScene::Draw()
{
    DrawString(40, 40, "Title Scene", GetColor(255, 255, 255));
    DrawString(40, 80, "Press ENTER to Start", GetColor(255, 255, 0));
    DrawString(40, 120, "Press ESC to Exit", GetColor(255, 180, 180));
    // TODO: タイトル背景やUI画像の描画処理をここに追加
}
