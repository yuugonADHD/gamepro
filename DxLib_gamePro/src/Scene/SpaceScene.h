#pragma once

#include "BaseScene.h"
#include "../Camera/Camera.h"

#include <vector>
#include <DxLib.h>

/**
 * @brief ワイヤーフレームスペースシューターシーン
 *
 * 自機が自動前進し、矢印キーで上下左右に移動。
 * スペースキーで弾を発射して障害物を破壊するデモシーン
 */
class SpaceScene : public BaseScene
{
public:
    /** @brief 障害物の種別 */
    enum class ObstacleType { Sphere, Box };

    /** @brief 弾のデータ */
    struct Bullet
    {
        VECTOR pos;   // ワールド座標
        bool   alive; // 生存フラグ
    };

    /** @brief 障害物のデータ */
    struct Obstacle
    {
        VECTOR       pos;          // ワールド座標
        ObstacleType type;         // 球 or 箱
        float        sphereRadius; // 球タイプの衝突判定半径
        VECTOR       boxHalf;      // 箱タイプのハーフサイズ
        bool         alive;        // 生存フラグ
    };

    /**
     * @brief コンストラクタ
     * @param sceneManager シーン遷移を管理するマネージャー
     * @return なし。
     */
    explicit SpaceScene(SceneManager* sceneManager);

    /**
     * @brief デストラクタ
     * @return なし。
     */
    ~SpaceScene() override;

    /**
     * @brief 初期化処理
     * @return なし。
     */
    void Init() override;

    /**
     * @brief 毎フレームの更新処理
     * @return なし。
     */
    void Update() override;

    /**
     * @brief 毎フレームの描画処理
     * @return なし。
     */
    void Draw() override;

private:
    // 障害物が不足していれば追加スポーン
    void SpawnObstaclesIfNeeded();

    // スクロールグリッドを描画
    void DrawGrid() const;

    Camera camera_;

    VECTOR playerPos_;   // 自機のワールド座標
    float  playerTiltX_; // 上下入力によるX軸バンク角（ラジアン）
    float  playerTiltZ_; // 左右入力によるZ軸バンク角（ラジアン）

    std::vector<Bullet>   bullets_;   // アクティブな弾リスト
    std::vector<Obstacle> obstacles_; // アクティブな障害物リスト

    int   lives_;      // 残りライフ
    int   startTime_;  // シーン開始時刻 (GetNowCount の戻り値)
    bool  isGameOver_; // ゲームオーバーフラグ
    bool  isClear_;    // クリアフラグ
    float endTimer_;   // 終了後のタイマー（秒）

    float deltaTime_;  // 1フレームの経過時間（秒）
};
