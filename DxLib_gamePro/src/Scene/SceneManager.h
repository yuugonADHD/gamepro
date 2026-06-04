#pragma once

#include <memory>

class BaseScene;

/**
 * @brief シーンの種類を表す列挙型
 */
enum class SceneID
{
    /** @brief タイトルシーン */
    Title,
    /** @brief 2Dゲーム本編シーン */
    Game2D,
    /** @brief 3Dゲーム本編シーン */
    Game3D,
    /** @brief スペースシューターシーン */
    Space,
    /** @brief リザルトシーン */
    Result
};

/**
 * @brief シーンの生成・切り替え・更新を担当するクラス
 */
class SceneManager
{
public:
    /**
     * @brief シーンマネージャーを生成
     * @return なし。
     */
    SceneManager();

    /**
     * @brief シーンマネージャーを破棄
     * @return なし。
     */
    ~SceneManager();

    /**
     * @brief 指定したシーンへ切り替え
     * @param sceneID 切り替え先のシーンID
     * @return なし。
     */
    void ChangeScene(SceneID sceneID);

    /**
     * @brief 現在シーンの更新処理
     * @return なし。
     */
    void Update();

    /**
     * @brief 現在シーンの描画処理
     * @return なし。
     */
    void Draw() const;

private:
    /** @brief 現在アクティブなシーン */
    std::unique_ptr<BaseScene> currentScene_;
};
