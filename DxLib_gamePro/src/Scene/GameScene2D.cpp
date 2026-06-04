#include "GameScene2D.h"

#include "../Input/InputManager.h"
#include "../Collision/Collision2D.h"
#include "../Common/Constants.h"
#include "SceneManager.h"

#include <algorithm>
#include <cstdlib>
#include <DxLib.h>

/**
 * @brief 2Dゲームシーンを生成
 * @param sceneManager シーン遷移を依頼する先のマネージャー
 * @return なし。
 */
GameScene2D::GameScene2D(SceneManager* sceneManager)
    : BaseScene(sceneManager)
{
}

/**
 * @brief 2Dゲームシーンを破棄
 * @return なし。
 */
GameScene2D::~GameScene2D() = default;

/**
 * @brief 2Dゲームシーンの初期化
 * @return なし。
 */
void GameScene2D::Init()
{
    // プレイヤーの初期位置・速度・サイズを設定
    playerX_ = 640.0f;
    playerY_ = 360.0f;
    playerSpeed_ = 5.0f;
    playerSize_ = 50;
    score_ = 0;

    // アイテムをランダム配置
    itemSize_ = 30;
    itemX_ = static_cast<float>(rand() % (Constants::SCREEN_WIDTH - itemSize_));
    itemY_ = static_cast<float>(rand() % (Constants::SCREEN_HEIGHT - itemSize_));
    itemActive_ = true;

    // 障害物をランダム配置（プレイヤー初期位置から200px以上離す）
    obstacleSize_ = 60;
    do
    {
        obstacleX_ = static_cast<float>(rand() % (Constants::SCREEN_WIDTH - obstacleSize_));
        obstacleY_ = static_cast<float>(rand() % (Constants::SCREEN_HEIGHT - obstacleSize_));

        const float diffX = obstacleX_ - playerX_;
        const float diffY = obstacleY_ - playerY_;
        if (diffX * diffX + diffY * diffY >= 200.0f * 200.0f)
        {
            break;
        }
    } while (true);
}

/**
 * @brief 2Dゲームシーンの更新処理
 * @return なし。
 */
void GameScene2D::Update()
{
    // プレイヤーが移動する前の座標を保存
    // 移動量を調べて、どちらの方向からぶつかったか判定するために使用
    const float prevX = playerX_;
    const float prevY = playerY_;

    // 矢印キーが押されている間、プレイヤーを上下左右に移動
    InputManager& input = InputManager::GetInstance();
    if (input.IsKeyHeld(KEY_INPUT_LEFT))
    {
        playerX_ -= playerSpeed_;
    }
    if (input.IsKeyHeld(KEY_INPUT_RIGHT))
    {
        playerX_ += playerSpeed_;
    }

    // まずは横方向の移動だけを画面内に収める
    playerX_ = std::clamp(playerX_, 0.0f, static_cast<float>(Constants::SCREEN_WIDTH) - static_cast<float>(playerSize_));

    // 横方向で障害物にぶつかったら、障害物の端にぴったり接する位置へ補正
    if (Collision2D::SquareToSquare(playerX_, prevY, playerSize_, obstacleX_, obstacleY_, obstacleSize_))
    {
        if (playerX_ > prevX)
        {
            playerX_ = obstacleX_ - static_cast<float>(playerSize_);
        }
        else if (playerX_ < prevX)
        {
            playerX_ = obstacleX_ + static_cast<float>(obstacleSize_);
        }
    }

    if (input.IsKeyHeld(KEY_INPUT_UP))
    {
        playerY_ -= playerSpeed_;
    }
    if (input.IsKeyHeld(KEY_INPUT_DOWN))
    {
        playerY_ += playerSpeed_;
    }

    // 次に縦方向の移動だけを画面内に収める
    playerY_ = std::clamp(playerY_, 0.0f, static_cast<float>(Constants::SCREEN_HEIGHT) - static_cast<float>(playerSize_));

    // 縦方向で障害物にぶつかったら、障害物の端にぴったり接する位置へ補正
    if (Collision2D::SquareToSquare(playerX_, playerY_, playerSize_, obstacleX_, obstacleY_, obstacleSize_))
    {
        if (playerY_ > prevY)
        {
            playerY_ = obstacleY_ - static_cast<float>(playerSize_);
        }
        else if (playerY_ < prevY)
        {
            playerY_ = obstacleY_ + static_cast<float>(obstacleSize_);
        }
    }

    // 念のため、補正後の座標も画面内に収める
    playerX_ = std::clamp(playerX_, 0.0f, static_cast<float>(Constants::SCREEN_WIDTH) - static_cast<float>(playerSize_));
    playerY_ = std::clamp(playerY_, 0.0f, static_cast<float>(Constants::SCREEN_HEIGHT) - static_cast<float>(playerSize_));

    // アイテムと重なったら取得成功
    // いったん非表示にしてスコアを増やし、そのあと新しい場所に再配置
    if (itemActive_ && Collision2D::SquareToSquare(playerX_, playerY_, playerSize_, itemX_, itemY_, itemSize_))
    {
        itemActive_ = false;
        score_ += 1;
        itemX_ = static_cast<float>(rand() % (Constants::SCREEN_WIDTH - itemSize_));
        itemY_ = static_cast<float>(rand() % (Constants::SCREEN_HEIGHT - itemSize_));
        itemActive_ = true;
    }

    // Enterキーを押した瞬間に3Dゲームシーンへ遷移
    if (input.IsKeyPressed(KEY_INPUT_RETURN))
    {
        sceneManager_->ChangeScene(SceneID::Game3D);
    }

    // TODO: アイテムの数を増やして配列で管理してみよう
    // TODO: 障害物に当たったらスコアを減らすなど、ルールを追加してみよう
}

/**
 * @brief 2Dゲームシーンの描画処理
 * @return なし。
 */
void GameScene2D::Draw()
{
    // 障害物は灰色の四角形として描画
    DrawBox(
        static_cast<int>(obstacleX_),
        static_cast<int>(obstacleY_),
        static_cast<int>(obstacleX_) + obstacleSize_,
        static_cast<int>(obstacleY_) + obstacleSize_,
        GetColor(128, 128, 128),
        TRUE);

    // アイテムが有効なときだけ、黄色の四角形を表示
    if (itemActive_)
    {
        DrawBox(
            static_cast<int>(itemX_),
            static_cast<int>(itemY_),
            static_cast<int>(itemX_) + itemSize_,
            static_cast<int>(itemY_) + itemSize_,
            GetColor(255, 255, 0),
            TRUE);
    }

    // プレイヤーは赤色の四角形として描画
    DrawBox(
        static_cast<int>(playerX_),
        static_cast<int>(playerY_),
        static_cast<int>(playerX_) + playerSize_,
        static_cast<int>(playerY_) + playerSize_,
        GetColor(255, 0, 0),
        TRUE);

    // 現在のスコアを画面の左上に表示
    DrawFormatString(10, 10, GetColor(255, 255, 255), "Score: %d", score_);

    // 次のシーンへの案内を表示
    DrawString(10, 40, "Press ENTER to 3D Scene", GetColor(255, 255, 0));
}
