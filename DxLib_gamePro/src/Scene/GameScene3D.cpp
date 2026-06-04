#include "GameScene3D.h"

#include "../Input/InputManager.h"
#include "../Primitive3D/Primitive3D.h"
#include "SceneManager.h"

#include <cmath>
#include <DxLib.h>

/** @brief 床の半幅（X方向） */
static constexpr float FLOOR_HALF_WIDTH  = 5.0f;
/** @brief 床の半高さ（Y方向） */
static constexpr float FLOOR_HALF_HEIGHT = 0.25f;
/** @brief 床の半奥行き（Z方向） */
static constexpr float FLOOR_HALF_DEPTH  = 5.0f;
/** @brief 床の中心Y座標 */
static constexpr float FLOOR_CENTER_Y    = 0.0f;
/** @brief 球の半径 */
static constexpr float BALL_RADIUS       = 0.5f;
/** @brief 重力加速度（ユニット/秒^2） */
static constexpr float GRAVITY           = 9.8f;
/** @brief 毎フレームの速度減衰係数（摩擦の簡易近似） */
static constexpr float DAMPING           = 0.98f;
/** @brief 床の傾き速度（ラジアン/秒） */
static constexpr float TILT_SPEED        = 1.0f;
/** @brief 床の最大傾き角（正負30度） */
static constexpr float TILT_MAX          = DX_PI_F / 6.0f;

/**
 * @brief 傾いた床を8頂点ボックスとして描画
 * @param rotX X軸まわりの回転角（ラジアン）。前後方向の傾き
 * @param rotZ Z軸まわりの回転角（ラジアン）。左右方向の傾き
 */
static void DrawTiltedFloor(float rotX, float rotZ)
{
    // ローカル座標での床のボックス8頂点
    VECTOR v[8] =
    {
        VGet(-FLOOR_HALF_WIDTH,  FLOOR_HALF_HEIGHT, -FLOOR_HALF_DEPTH),
        VGet( FLOOR_HALF_WIDTH,  FLOOR_HALF_HEIGHT, -FLOOR_HALF_DEPTH),
        VGet( FLOOR_HALF_WIDTH,  FLOOR_HALF_HEIGHT,  FLOOR_HALF_DEPTH),
        VGet(-FLOOR_HALF_WIDTH,  FLOOR_HALF_HEIGHT,  FLOOR_HALF_DEPTH),
        VGet(-FLOOR_HALF_WIDTH, -FLOOR_HALF_HEIGHT, -FLOOR_HALF_DEPTH),
        VGet( FLOOR_HALF_WIDTH, -FLOOR_HALF_HEIGHT, -FLOOR_HALF_DEPTH),
        VGet( FLOOR_HALF_WIDTH, -FLOOR_HALF_HEIGHT,  FLOOR_HALF_DEPTH),
        VGet(-FLOOR_HALF_WIDTH, -FLOOR_HALF_HEIGHT,  FLOOR_HALF_DEPTH),
    };

    // X軸・Z軸の回転行列を合成して各頂点に適用
    const MATRIX mat = MMult(
        MMult(MGetRotX(rotX), MGetRotZ(rotZ)),
        MGetTranslate(VGet(0.0f, FLOOR_CENTER_Y, 0.0f)));
    for (int i = 0; i < 8; i++) { v[i] = VTransform(v[i], mat); }

    // 上面をベージュ色で描画（三角形2枚）
    const unsigned int faceColor = GetColor(220, 200, 140);
    DrawTriangle3D(v[0], v[1], v[2], faceColor, TRUE);
    DrawTriangle3D(v[0], v[2], v[3], faceColor, TRUE);

    // 側面を少し暗い色で描画（4面 × 三角形2枚）
    const unsigned int sideColor = GetColor(160, 140, 90);
    DrawTriangle3D(v[0], v[1], v[5], sideColor, TRUE);
    DrawTriangle3D(v[0], v[5], v[4], sideColor, TRUE);
    DrawTriangle3D(v[1], v[2], v[6], sideColor, TRUE);
    DrawTriangle3D(v[1], v[6], v[5], sideColor, TRUE);
    DrawTriangle3D(v[2], v[3], v[7], sideColor, TRUE);
    DrawTriangle3D(v[2], v[7], v[6], sideColor, TRUE);
    DrawTriangle3D(v[3], v[0], v[4], sideColor, TRUE);
    DrawTriangle3D(v[3], v[4], v[7], sideColor, TRUE);

    // 上面の稜線を濃い線で描画
    const unsigned int edgeColor = GetColor(40, 40, 40);
    DrawLine3D(v[0], v[1], edgeColor);
    DrawLine3D(v[1], v[2], edgeColor);
    DrawLine3D(v[2], v[3], edgeColor);
    DrawLine3D(v[3], v[0], edgeColor);
}

/**
 * @brief コンストラクタ
 */
GameScene3D::GameScene3D(SceneManager* sceneManager)
    : BaseScene(sceneManager)
    , floorRotX_(0.0f)
    , floorRotZ_(0.0f)
    , ballPos_(VGet(0.0f, FLOOR_CENTER_Y + FLOOR_HALF_HEIGHT + BALL_RADIUS, 0.0f))
    , ballVel_(VGet(0.0f, 0.0f, 0.0f))
    , deltaTime_(1.0f / 60.0f)
{
}

GameScene3D::~GameScene3D() = default;

/**
 * @brief 初期化処理
 */
void GameScene3D::Init()
{
    // 床の傾きとボールの状態をリセット
    floorRotX_ = 0.0f;
    floorRotZ_ = 0.0f;
    ballPos_   = VGet(0.0f, FLOOR_CENTER_Y + FLOOR_HALF_HEIGHT + BALL_RADIUS, 0.0f);
    ballVel_   = VGet(0.0f, 0.0f, 0.0f);

    light_.Init();

    // 床を斜め上から見下ろす固定カメラ
    camera_.SetPosition(VGet(0.0f, 18.0f, -18.0f));
    camera_.SetTarget(VGet(0.0f, 0.0f, 0.0f));

    // カメラをDxLibに反映
    camera_.Update();
}

/**
 * @brief 毎フレームの更新処理
 */
void GameScene3D::Update()
{
    // カメラをDxLibに反映
    camera_.Update();

    InputManager& input = InputManager::GetInstance();

    // 上下キー: X軸まわりに床を傾ける（前後方向の傾き）
    if (input.IsKeyHeld(KEY_INPUT_UP))    { floorRotX_ += TILT_SPEED * deltaTime_; }
    if (input.IsKeyHeld(KEY_INPUT_DOWN))  { floorRotX_ -= TILT_SPEED * deltaTime_; }

    // 左右キー: Z軸まわりに床を傾ける（左右方向の傾き）
    if (input.IsKeyHeld(KEY_INPUT_LEFT))  { floorRotZ_ += TILT_SPEED * deltaTime_; }
    if (input.IsKeyHeld(KEY_INPUT_RIGHT)) { floorRotZ_ -= TILT_SPEED * deltaTime_; }

    // 傾き角を上限値にクランプ
    if (floorRotX_ >  TILT_MAX) floorRotX_ =  TILT_MAX;
    if (floorRotX_ < -TILT_MAX) floorRotX_ = -TILT_MAX;
    if (floorRotZ_ >  TILT_MAX) floorRotZ_ =  TILT_MAX;
    if (floorRotZ_ < -TILT_MAX) floorRotZ_ = -TILT_MAX;

    // --- ボールの物理シミュレーション ---
    //
    // 床が傾くと重力の一部が床面に平行に働き、ボールを加速させる
    //
    //   X軸傾き (floorRotX_) → Z方向の見かけの重力 = G * sin(rotX)
    //   Z軸傾き (floorRotZ_) → X方向の見かけの重力 = G * sin(rotZ)

    ballVel_.x += -sinf(floorRotZ_) * GRAVITY * deltaTime_;
    ballVel_.z +=  sinf(floorRotX_) * GRAVITY * deltaTime_;

    // Y方向に下向きの重力を毎フレーム加算
    ballVel_.y -= GRAVITY * deltaTime_;


    // 速度に減衰をかけて転がり摩擦を再現
    ballVel_.x *= DAMPING;
    ballVel_.z *= DAMPING;

    // 速度を位置に積分
    ballPos_.x += ballVel_.x * deltaTime_;
    ballPos_.y += ballVel_.y * deltaTime_;
    ballPos_.z += ballVel_.z * deltaTime_;

    // --- 床面のY座標を算出 ---
    //
    // 床の平面をX軸・Z軸まわりに回転させたもの
    // 球のXZ位置における床面のY座標を平面方程式から求める
    //
    // 未回転の床の法線: (0, 1, 0)
    // 回転後の法線: normal = rotMat * (0, 1, 0)
    //
    // 平面方程式（標準形）:
    //   normal.x * x + normal.y * y + normal.z * z = FLOOR_CENTER_Y
    //
    // 球のXZ位置でyを解く:
    //   y = (FLOOR_CENTER_Y - normal.x * x - normal.z * z) / normal.y
    const MATRIX rotMat   = MMult(MGetRotX(floorRotX_), MGetRotZ(floorRotZ_));
    const VECTOR normal   = VTransform(VGet(0.0f, 1.0f, 0.0f), rotMat);
    float floorSurfaceY   = FLOOR_CENTER_Y;
    if (fabsf(normal.y) > 0.001f)
    {
        floorSurfaceY = (FLOOR_CENTER_Y
                        - normal.x * ballPos_.x
                        - normal.z * ballPos_.z) / normal.y;
    }

    // 床のローカル座標系に変換するため逆回転行列を取得
    const MATRIX invRotMat = MMult(MGetRotZ(-floorRotZ_), MGetRotX(-floorRotX_));
    const VECTOR localPos  = VTransform(
        VGet(ballPos_.x, ballPos_.y, ballPos_.z),
        invRotMat);

    // 球が床の範囲内にあるか判定（ローカル座標でのXZ矩形チェック）
    const bool onFloor = (fabsf(localPos.x) <= FLOOR_HALF_WIDTH &&
                          fabsf(localPos.z) <= FLOOR_HALF_DEPTH);

    // 球が床にめり込んでいた場合、床面の上に押し戻す
    // 床の範囲外なら摩擦なしで自由落下
    if (onFloor)
    {
        const float contactY = floorSurfaceY + FLOOR_HALF_HEIGHT + BALL_RADIUS;
        if (ballPos_.y <= contactY)
        {
            ballPos_.y = contactY;
            ballVel_.y = 0.0f;
        }
    }

    // Enterキーを押した瞬間にスペースシーンへ遷移
    if (InputManager::GetInstance().IsKeyPressed(KEY_INPUT_RETURN))
    {
        sceneManager_->ChangeScene(SceneID::Space);
    }
}

/**
 * @brief 毎フレームの描画処理
 */
void GameScene3D::Draw()
{
    // 傾いた床を描画
    DrawTiltedFloor(floorRotX_, floorRotZ_);

    // 球を赤い塗りつぶし球として描画
    Primitive3D::DrawSphere(ballPos_, BALL_RADIUS, GetColorU8(240, 20, 20, 255), true);

    DrawString(10, 10, "3D Rolling Ball",            GetColor(255, 255, 255));
    DrawString(10, 40, "Arrow Keys: Tilt the floor", GetColor(255, 255,   0));
}
