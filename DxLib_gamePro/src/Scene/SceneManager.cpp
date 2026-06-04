#include "SceneManager.h"

#include "BaseScene.h"
#include "GameScene2D.h"
#include "GameScene3D.h"
#include "SpaceScene.h"
#include "ResultScene.h"
#include "TitleScene.h"

/**
 * @brief シーンマネージャーを生成
 * @return なし。
 */
SceneManager::SceneManager() = default;

/**
 * @brief シーンマネージャーを破棄
 * @return なし。
 */
SceneManager::~SceneManager() = default;

/**
 * @brief 指定したシーンへ切り替え
 * @param sceneID 切り替え先のシーンID
 * @return なし。
 */
void SceneManager::ChangeScene(const SceneID sceneID)
{
    switch (sceneID)
    {
    case SceneID::Title:
        currentScene_ = std::make_unique<TitleScene>(this);
        break;
    case SceneID::Game2D:
        currentScene_ = std::make_unique<GameScene2D>(this);
        break;
    case SceneID::Game3D:
        currentScene_ = std::make_unique<GameScene3D>(this);
        break;
    case SceneID::Space:
        currentScene_ = std::make_unique<SpaceScene>(this);
        break;
    case SceneID::Result:
        currentScene_ = std::make_unique<ResultScene>(this);
        break;
    default:
        return;
    }

    if (currentScene_)
    {
        currentScene_->Init();
    }
}

/**
 * @brief 現在シーンの更新処理
 * @return なし。
 */
void SceneManager::Update()
{
    if (currentScene_)
    {
        currentScene_->Update();
    }
}

/**
 * @brief 現在シーンの描画処理
 * @return なし。
 */
void SceneManager::Draw() const
{
    if (currentScene_)
    {
        currentScene_->Draw();
    }
}
