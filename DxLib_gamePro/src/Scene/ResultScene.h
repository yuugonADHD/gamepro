#pragma once

#include "BaseScene.h"

/**
 * @brief リザルト画面を表すシーンクラス
 */
class ResultScene : public BaseScene
{
public:
    /**
     * @brief リザルトシーンを生成
     * @param sceneManager シーン遷移を担うシーンマネージャー
     * @return なし。
     */
    explicit ResultScene(SceneManager* sceneManager);

    /**
     * @brief リザルトシーンを破棄
     * @return なし。
     */
    ~ResultScene() override;

    /**
     * @brief リザルトシーンの初期化
     * @return なし。
     */
    void Init() override;

    /**
     * @brief リザルトシーンの更新処理
     * @return なし。
     */
    void Update() override;

    /**
     * @brief リザルトシーンの描画処理
     * @return なし。
     */
    void Draw() override;
};
