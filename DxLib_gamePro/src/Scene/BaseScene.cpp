#include "BaseScene.h"

/**
 * @brief 基底シーンクラスを初期化
 * @param sceneManager シーン遷移を依頼する先のマネージャー
 * @return なし。
 */
BaseScene::BaseScene(SceneManager* sceneManager)
    : sceneManager_(sceneManager)
{
}

/**
 * @brief 基底シーンクラスを破棄
 * @return なし。
 */
BaseScene::~BaseScene() = default;
