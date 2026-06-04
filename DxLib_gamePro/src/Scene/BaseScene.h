#pragma once

class SceneManager;

/**
 * @brief すべてのシーンの基底クラス
 */
class BaseScene
{
public:
    /**
     * @brief 基底シーンクラスを初期化
     * @param sceneManager シーン遷移を依頼する先のマネージャー
     * @return なし。
     */
    explicit BaseScene(SceneManager* sceneManager);

    /**
     * @brief 基底シーンクラスを破棄
     * @return なし。
     */
    virtual ~BaseScene();

    /**
     * @brief シーンの初期化処理
     * @return なし。
     */
    virtual void Init() = 0;

    /**
     * @brief シーンの更新処理
     * @return なし。
     */
    virtual void Update() = 0;

    /**
     * @brief シーンの描画処理
     * @return なし。
     */
    virtual void Draw() = 0;

protected:
    /** @brief シーン遷移を依頼するためのマネージャーポインタ */
    SceneManager* sceneManager_;
};
