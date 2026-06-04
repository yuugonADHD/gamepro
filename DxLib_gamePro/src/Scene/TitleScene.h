#pragma once

#include "BaseScene.h"

/**
 * @brief タイトル画面を表すシーンクラス
 */
class TitleScene : public BaseScene
{
public:
    /**
     * @brief タイトルシーンを生成
     * @param sceneManager シーン遷移を担うシーンマネージャー
     * @return なし。
     */
    explicit TitleScene(SceneManager* sceneManager);

    /**
     * @brief タイトルシーンを破棄
     * @return なし。
     */
    ~TitleScene() override;

    /**
     * @brief タイトルシーンの初期化
     * @return なし。
     */
    void Init() override;

    /**
     * @brief タイトルシーンの更新処理
     * @return なし。
     */
    void Update() override;

    /**
     * @brief タイトルシーンの描画処理
     * @return なし。
     */
    void Draw() override;
};
