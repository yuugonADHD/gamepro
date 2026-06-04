#pragma once

#include "BaseScene.h"
#include "../Camera/Camera.h"
#include "../Light/Light.h"

#include <DxLib.h>

/**
 * @brief 3Dゲーム本編を表すシーンクラス
 *
 * 矢印キーで床を傾けて球を転がすデモシーン
 */
class GameScene3D : public BaseScene
{
public:
    /**
     * @brief コンストラクタ
     * @param sceneManager シーン遷移を管理するマネージャー
     * @return なし。
     */
    explicit GameScene3D(SceneManager* sceneManager);

    /**
     * @brief デストラクタ
     * @return なし。
     */
    ~GameScene3D() override;

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
    /** @brief 3Dカメラ */
    Camera camera_;

    /** @brief ライト */
    Light light_;

    /** @brief 床のX軸まわりの傾き角（ラジアン）。前後方向の傾き */
    float floorRotX_;

    /** @brief 床のZ軸まわりの傾き角（ラジアン）。左右方向の傾き */
    float floorRotZ_;

    /** @brief 球のワールド座標 */
    VECTOR ballPos_;

    /** @brief 球の速度ベクトル（単位: ユニット/秒） */
    VECTOR ballVel_;

    /** @brief 1フレームあたりの経過時間（秒） */
    float deltaTime_;
};
