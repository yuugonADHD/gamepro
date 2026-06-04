#pragma once

#include "BaseScene.h"

/**
 * @brief 2Dゲーム本編を表すシーンクラス
 *
 * 矢印キーでプレイヤーを動かし、アイテムを取得するシンプルな2Dゲーム
 * このシーンをクリアするとGameScene3Dへ遷移
 */
class GameScene2D : public BaseScene
{
public:
    /**
     * @brief 2Dゲームシーンを生成
     * @param sceneManager シーン遷移を依頼する先のマネージャー
     * @return なし。
     */
    explicit GameScene2D(SceneManager* sceneManager);

    /**
     * @brief 2Dゲームシーンを破棄
     * @return なし。
     */
    ~GameScene2D() override;

    /**
     * @brief 2Dゲームシーンの初期化
     * @return なし。
     */
    void Init() override;

    /**
     * @brief 2Dゲームシーンの更新処理
     * @return なし。
     */
    void Update() override;

    /**
     * @brief 2Dゲームシーンの描画処理
     * @return なし。
     */
    void Draw() override;

private:
    /** @brief プレイヤーのX座標 */
    float playerX_;

    /** @brief プレイヤーのY座標 */
    float playerY_;

    /** @brief プレイヤーの移動速度 */
    float playerSpeed_;

    /** @brief プレイヤーの大きさ */
    int playerSize_;

    /** @brief アイテムのX座標 */
    float itemX_;

    /** @brief アイテムのY座標 */
    float itemY_;

    /** @brief アイテムの大きさ */
    int itemSize_;

    /** @brief アイテムの有効フラグ */
    bool itemActive_;

    /** @brief 障害物のX座標 */
    float obstacleX_;

    /** @brief 障害物のY座標 */
    float obstacleY_;

    /** @brief 障害物の大きさ */
    int obstacleSize_;

    /** @brief 取得したアイテム数を表すスコア */
    int score_;
};
