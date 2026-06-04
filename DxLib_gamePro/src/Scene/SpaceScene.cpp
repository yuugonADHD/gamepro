#include "SpaceScene.h"

#include "../Input/InputManager.h"
#include "../Primitive3D/Primitive3D.h"
#include "../Collision/Collision3D.h"
#include "SceneManager.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <DxLib.h>

// ============================================================
// 定数
// ============================================================

/** @brief 自機の前進速度（ユニット/秒） */
static constexpr float PLAYER_SPEED       = 20.0f;
/** @brief 上下左右の移動速度（ユニット/秒） */
static constexpr float PLAYER_MOVE_SPEED  = 8.0f;
/** @brief 移動範囲の制限 */
static constexpr float PLAYER_CLAMP       = 8.0f;
/** @brief バンク傾きの変化速度（ラジアン/秒） */
static constexpr float PLAYER_TILT_SPEED  = 4.0f;
/** @brief バンクの最大傾き角（ラジアン） */
static constexpr float PLAYER_TILT_MAX    = 0.6f;
/** @brief バンクが0に戻る速度（ラジアン/秒） */
static constexpr float PLAYER_TILT_RETURN = 6.0f;
/** @brief 自機の衝突判定半径 */
static constexpr float PLAYER_RADIUS      = 0.9f;

/** @brief 自機ボックスのハーフサイズ */
static constexpr float SHIP_HX = 0.8f;
static constexpr float SHIP_HY = 0.4f;
static constexpr float SHIP_HZ = 1.2f;

/** @brief 弾の前進速度（ユニット/秒） */
static constexpr float BULLET_SPEED     = 50.0f;
/** @brief 弾の衝突判定半径 */
static constexpr float BULLET_RADIUS    = 0.3f;
/** @brief 弾の最大射程（プレイヤーからの前方距離） */
static constexpr float BULLET_MAX_RANGE = 60.0f;

/** @brief 同時出現する障害物の目標数 */
static constexpr int   OBSTACLE_COUNT       = 12;
/** @brief 障害物のスポーン距離（プレイヤー前方） */
static constexpr float OBSTACLE_SPAWN_DIST  = 80.0f;
/** @brief スポーン時のランダムZ散布幅 */
static constexpr float OBSTACLE_SPAWN_SCATTER = 50.0f;
/** @brief スポーンX範囲（±） */
static constexpr float OBSTACLE_RANGE_X     = 9.0f;
/** @brief スポーンY範囲（±） */
static constexpr float OBSTACLE_RANGE_Y     = 7.0f;
/** @brief この距離だけ後方に過ぎた障害物を削除 */
static constexpr float OBSTACLE_REMOVE_DIST = 5.0f;

/** @brief 初期ライフ数 */
static constexpr int   INITIAL_LIVES     = 3;
/** @brief ゲームの制限時間（秒） */
static constexpr float GAME_DURATION_SEC = 60.0f;
/** @brief ゲーム終了後にリザルト画面へ遷移するまでの待機時間（秒） */
static constexpr float END_WAIT_SEC      = 2.0f;

/** @brief グリッドの間隔幅（ユニット） */
static constexpr float GRID_SPACING = 6.0f;
/** @brief 片側のグリッド線本数 */
static constexpr int   GRID_COUNT   = 8;
/** @brief グリッドのY座標 */
static constexpr float GRID_Y       = -5.0f;

/** @brief 固定デルタタイム（秒） */
static constexpr float DELTA_TIME = 1.0f / 60.0f;

// ============================================================
// ファイルローカルヘルパー関数
// ============================================================

/**
 * @brief 自機を回転させたワイヤーフレームボックスとして描画
 * @param pos    自機のワールド座標
 * @param tiltX  X軸バンク角（ラジアン）
 * @param tiltZ  Z軸バンク角（ラジアン）
 */
static void DrawPlayerShip(VECTOR pos, float tiltX, float tiltZ)
{
    // ローカル座標系でのボックス8頂点
    VECTOR v[8] =
    {
        VGet(-SHIP_HX,  SHIP_HY, -SHIP_HZ),
        VGet( SHIP_HX,  SHIP_HY, -SHIP_HZ),
        VGet( SHIP_HX,  SHIP_HY,  SHIP_HZ),
        VGet(-SHIP_HX,  SHIP_HY,  SHIP_HZ),
        VGet(-SHIP_HX, -SHIP_HY, -SHIP_HZ),
        VGet( SHIP_HX, -SHIP_HY, -SHIP_HZ),
        VGet( SHIP_HX, -SHIP_HY,  SHIP_HZ),
        VGet(-SHIP_HX, -SHIP_HY,  SHIP_HZ),
    };

    // バンク回転後にワールド移動を適用
    const MATRIX mat = MMult(
        MMult(MGetRotX(tiltX), MGetRotZ(tiltZ)),
        MGetTranslate(pos));
    for (int i = 0; i < 8; i++) { v[i] = VTransform(v[i], mat); }

    const unsigned int c = GetColor(0, 255, 128); // 緑がかった白

    // 上面
    DrawLine3D(v[0], v[1], c); DrawLine3D(v[1], v[2], c);
    DrawLine3D(v[2], v[3], c); DrawLine3D(v[3], v[0], c);
    // 下面
    DrawLine3D(v[4], v[5], c); DrawLine3D(v[5], v[6], c);
    DrawLine3D(v[6], v[7], c); DrawLine3D(v[7], v[4], c);
    // 縦柱
    DrawLine3D(v[0], v[4], c); DrawLine3D(v[1], v[5], c);
    DrawLine3D(v[2], v[6], c); DrawLine3D(v[3], v[7], c);
}

// ============================================================
// SpaceScene メンバー関数
// ============================================================

SpaceScene::SpaceScene(SceneManager* sceneManager)
    : BaseScene(sceneManager)
    , playerPos_(VGet(0.0f, 0.0f, 0.0f))
    , playerTiltX_(0.0f)
    , playerTiltZ_(0.0f)
    , lives_(INITIAL_LIVES)
    , startTime_(0)
    , isGameOver_(false)
    , isClear_(false)
    , endTimer_(0.0f)
    , deltaTime_(DELTA_TIME)
{
}

SpaceScene::~SpaceScene() = default;

void SpaceScene::Init()
{
    // 全状態のリセット
    playerPos_   = VGet(0.0f, 0.0f, 0.0f);
    playerTiltX_ = 0.0f;
    playerTiltZ_ = 0.0f;
    lives_       = INITIAL_LIVES;
    startTime_   = GetNowCount();
    isGameOver_  = false;
    isClear_     = false;
    endTimer_    = 0.0f;
    bullets_.clear();
    obstacles_.clear();

    // カメラを自機の後方上方に配置
    camera_.Follow(playerPos_, VGet(0.0f, 3.0f, -10.0f));
    camera_.Update();

    // 初期障害物をスポーン
    SpawnObstaclesIfNeeded();
}

void SpaceScene::Update()
{
    // --- 終了状態の処理 ---
    // メッセージを表示する間、少し待ってからシーン遷移
    if (isGameOver_ || isClear_)
    {
        endTimer_ += deltaTime_;
        if (endTimer_ >= END_WAIT_SEC)
        {
            sceneManager_->ChangeScene(SceneID::Result);
        }
        return;
    }

    // --- タイマー ---
    const float elapsed   = static_cast<float>(GetNowCount() - startTime_) / 1000.0f;
    const float remaining = GAME_DURATION_SEC - elapsed;

    if (remaining <= 0.0f) { isClear_   = true; return; }
    if (lives_     <= 0)   { isGameOver_ = true; return; }

    // --- カメラ追従 ---
    camera_.Follow(playerPos_, VGet(0.0f, 3.0f, -10.0f));
    camera_.Update();

    // --- 自機前進 ---
    playerPos_.z += PLAYER_SPEED * deltaTime_;

    // --- 入力処理 ---
    InputManager& input = InputManager::GetInstance();

    const bool moveLeft  = input.IsKeyHeld(KEY_INPUT_LEFT);
    const bool moveRight = input.IsKeyHeld(KEY_INPUT_RIGHT);
    const bool moveUp    = input.IsKeyHeld(KEY_INPUT_UP);
    const bool moveDown  = input.IsKeyHeld(KEY_INPUT_DOWN);

    if (moveLeft)  { playerPos_.x -= PLAYER_MOVE_SPEED * deltaTime_; }
    if (moveRight) { playerPos_.x += PLAYER_MOVE_SPEED * deltaTime_; }
    if (moveUp)    { playerPos_.y += PLAYER_MOVE_SPEED * deltaTime_; }
    if (moveDown)  { playerPos_.y -= PLAYER_MOVE_SPEED * deltaTime_; }

    // XY位置のクランプ
    if (playerPos_.x >  PLAYER_CLAMP) playerPos_.x =  PLAYER_CLAMP;
    if (playerPos_.x < -PLAYER_CLAMP) playerPos_.x = -PLAYER_CLAMP;
    if (playerPos_.y >  PLAYER_CLAMP) playerPos_.y =  PLAYER_CLAMP;
    if (playerPos_.y < -PLAYER_CLAMP) playerPos_.y = -PLAYER_CLAMP;

    // --- バンク傾きの更新 ---
    // 左右入力: Z軸傾き
    if (moveLeft)       { playerTiltZ_ += PLAYER_TILT_SPEED  * deltaTime_; }
    else if (moveRight) { playerTiltZ_ -= PLAYER_TILT_SPEED  * deltaTime_; }
    else
    {
        // 入力なし: 0に向かって自動復帰
        if (playerTiltZ_ > 0.0f) { playerTiltZ_ -= PLAYER_TILT_RETURN * deltaTime_; if (playerTiltZ_ < 0.0f) playerTiltZ_ = 0.0f; }
        if (playerTiltZ_ < 0.0f) { playerTiltZ_ += PLAYER_TILT_RETURN * deltaTime_; if (playerTiltZ_ > 0.0f) playerTiltZ_ = 0.0f; }
    }
    // 上下入力: X軸傾き
    if (moveUp)        { playerTiltX_ -= PLAYER_TILT_SPEED  * deltaTime_; }
    else if (moveDown) { playerTiltX_ += PLAYER_TILT_SPEED  * deltaTime_; }
    else
    {
        if (playerTiltX_ > 0.0f) { playerTiltX_ -= PLAYER_TILT_RETURN * deltaTime_; if (playerTiltX_ < 0.0f) playerTiltX_ = 0.0f; }
        if (playerTiltX_ < 0.0f) { playerTiltX_ += PLAYER_TILT_RETURN * deltaTime_; if (playerTiltX_ > 0.0f) playerTiltX_ = 0.0f; }
    }
    // バンク角のクランプ
    if (playerTiltZ_ >  PLAYER_TILT_MAX) playerTiltZ_ =  PLAYER_TILT_MAX;
    if (playerTiltZ_ < -PLAYER_TILT_MAX) playerTiltZ_ = -PLAYER_TILT_MAX;
    if (playerTiltX_ >  PLAYER_TILT_MAX) playerTiltX_ =  PLAYER_TILT_MAX;
    if (playerTiltX_ < -PLAYER_TILT_MAX) playerTiltX_ = -PLAYER_TILT_MAX;

    // --- スペースキーで弾を発射 ---
    if (input.IsKeyPressed(KEY_INPUT_SPACE))
    {
        Bullet b;
        b.pos   = playerPos_;
        b.alive = true;
        bullets_.push_back(b);
    }

    // --- 弾の位置を更新 ---
    for (auto& b : bullets_)
    {
        if (b.alive) { b.pos.z += BULLET_SPEED * deltaTime_; }
    }

    // --- 射程外または破壊済みの弾を削除 ---
    bullets_.erase(
        std::remove_if(bullets_.begin(), bullets_.end(),
            [&](const Bullet& b)
            {
                return !b.alive || b.pos.z > playerPos_.z + BULLET_MAX_RANGE;
            }),
        bullets_.end());

    // --- 後方に通り過ぎた障害物を削除 ---
    obstacles_.erase(
        std::remove_if(obstacles_.begin(), obstacles_.end(),
            [&](const Obstacle& o)
            {
                return !o.alive || o.pos.z < playerPos_.z - OBSTACLE_REMOVE_DIST;
            }),
        obstacles_.end());

    // 目標数を下回っていれば障害物を追加スポーン
    SpawnObstaclesIfNeeded();

    // --- 弾と障害物の衝突判定 ---
    for (auto& bullet : bullets_)
    {
        if (!bullet.alive) continue;
        for (auto& obs : obstacles_)
        {
            if (!obs.alive) continue;
            bool hit = false;
            if (obs.type == ObstacleType::Sphere)
            {
                hit = Collision3D::SphereToSphere(
                    SphereCollider(BULLET_RADIUS), bullet.pos,
                    SphereCollider(obs.sphereRadius), obs.pos);
            }
            else
            {
                hit = Collision3D::SphereToAABB(
                    SphereCollider(BULLET_RADIUS), bullet.pos,
                    AABBCollider(obs.boxHalf),     obs.pos);
            }
            if (hit)
            {
                obs.alive    = false;
                bullet.alive = false;
                break; // 1弾1ヒット
            }
        }
    }

    // --- 自機と障害物の衝突判定 ---
    for (auto& obs : obstacles_)
    {
        if (!obs.alive) continue;
        bool hit = false;
        if (obs.type == ObstacleType::Sphere)
        {
            hit = Collision3D::SphereToSphere(
                SphereCollider(PLAYER_RADIUS),    playerPos_,
                SphereCollider(obs.sphereRadius), obs.pos);
        }
        else
        {
            hit = Collision3D::SphereToAABB(
                SphereCollider(PLAYER_RADIUS), playerPos_,
                AABBCollider(obs.boxHalf),     obs.pos);
        }
        if (hit)
        {
            obs.alive = false;
            lives_--;
        }
    }

    // Enterキーを押した瞬間にリザルト画面へ遷移
    if (InputManager::GetInstance().IsKeyPressed(KEY_INPUT_RETURN))
    {
        sceneManager_->ChangeScene(SceneID::Result);
    }
}

void SpaceScene::Draw()
{
    // スクロールグリッドを描画
    DrawGrid();

    // 自機を描画
    DrawPlayerShip(playerPos_, playerTiltX_, playerTiltZ_);

    // 弾をシアンのワイヤーフレーム球で描画
    const COLOR_U8 bulletColor = GetColorU8(0, 220, 255, 255);
    for (const auto& b : bullets_)
    {
        if (b.alive)
        {
            Primitive3D::DrawSphere(b.pos, BULLET_RADIUS, bulletColor, false);
        }
    }

    // 障害物をオレンジのワイヤーフレームで描画
    const COLOR_U8 obsColor = GetColorU8(255, 160, 0, 255);
    for (const auto& obs : obstacles_)
    {
        if (!obs.alive) continue;
        if (obs.type == ObstacleType::Sphere)
        {
            Primitive3D::DrawSphere(obs.pos, obs.sphereRadius, obsColor, false);
        }
        else
        {
            Primitive3D::DrawBox(
                VSub(obs.pos, obs.boxHalf),
                VAdd(obs.pos, obs.boxHalf),
                obsColor, false);
        }
    }

    // --- UI テキスト ---
    const float elapsed   = static_cast<float>(GetNowCount() - startTime_) / 1000.0f;
    const float remaining = GAME_DURATION_SEC - elapsed;
    const float dispTime  = remaining > 0.0f ? remaining : 0.0f;

    DrawFormatString(10,  10, GetColor(255, 255, 255), "Lives: %d",      lives_);
    DrawFormatString(10,  40, GetColor(255, 255,   0), "Time:  %.0f s",  dispTime);
    DrawFormatString(10,  70, GetColor(180, 180, 255), "Space: Shoot | Arrows: Move", 0);

    if (isGameOver_)
    {
        DrawString(520, 340, "GAME OVER", GetColor(255, 60, 60));
    }
    else if (isClear_)
    {
        DrawString(545, 340, "CLEAR!",    GetColor(0, 255, 128));
    }
}

void SpaceScene::SpawnObstaclesIfNeeded()
{
    // OBSTACLE_COUNT 個になるまで障害物をスポーン
    while (static_cast<int>(obstacles_.size()) < OBSTACLE_COUNT)
    {
        Obstacle obs;
        obs.alive = true;

        // プレイ範囲内でランダムなXY位置
        const float rx = static_cast<float>(rand() % 2001) / 1000.0f - 1.0f; // -1 ~ +1
        const float ry = static_cast<float>(rand() % 2001) / 1000.0f - 1.0f;
        obs.pos.x = rx * OBSTACLE_RANGE_X;
        obs.pos.y = ry * OBSTACLE_RANGE_Y;

        // ランダムな散布を加えて前方にスポーン
        const float scatter = static_cast<float>(rand() % static_cast<int>(OBSTACLE_SPAWN_SCATTER * 10)) / 10.0f;
        obs.pos.z = playerPos_.z + OBSTACLE_SPAWN_DIST + scatter;

        // 球か箱かをランダムに決定
        if (rand() % 2 == 0)
        {
            obs.type         = ObstacleType::Sphere;
            obs.sphereRadius = 0.8f + static_cast<float>(rand() % 13) / 10.0f; // 0.8 ~ 2.0
            obs.boxHalf      = VGet(1.0f, 1.0f, 1.0f); // 未使用だが初期化
        }
        else
        {
            obs.type         = ObstacleType::Box;
            obs.sphereRadius = 1.0f; // 未使用だが初期化
            const float bx = 0.7f + static_cast<float>(rand() % 14) / 10.0f; // 0.7 ~ 2.0
            const float by = 0.7f + static_cast<float>(rand() % 14) / 10.0f;
            const float bz = 0.7f + static_cast<float>(rand() % 14) / 10.0f;
            obs.boxHalf = VGet(bx, by, bz);
        }

        obstacles_.push_back(obs);
    }
}

void SpaceScene::DrawGrid() const
{
    const unsigned int col = GetColor(0, 100, 255);

    // プレイヤーのZ座標をグリッド間隔でモジュロしてオフセット算出（シームレスなスクロール）
    const float zOffset = fmodf(playerPos_.z, GRID_SPACING);

    for (int i = -GRID_COUNT; i <= GRID_COUNT; i++)
    {
        // Z方向の縦線
        const float x     = playerPos_.x + static_cast<float>(i) * GRID_SPACING;
        const float zNear = playerPos_.z - static_cast<float>(GRID_COUNT) * GRID_SPACING;
        const float zFar  = playerPos_.z + static_cast<float>(GRID_COUNT) * GRID_SPACING;
        DrawLine3D(VGet(x, GRID_Y, zNear), VGet(x, GRID_Y, zFar), col);

        // X方向の横線（プレイヤーに合わせてスクロール）
        const float z     = playerPos_.z + static_cast<float>(i) * GRID_SPACING - zOffset;
        const float xNear = playerPos_.x - static_cast<float>(GRID_COUNT) * GRID_SPACING;
        const float xFar  = playerPos_.x + static_cast<float>(GRID_COUNT) * GRID_SPACING;
        DrawLine3D(VGet(xNear, GRID_Y, z), VGet(xFar, GRID_Y, z), col);
    }
}
