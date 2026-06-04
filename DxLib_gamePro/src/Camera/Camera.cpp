#include "Camera.h"

/**
 * @brief カメラを生成し、デフォルト値を設定
 */
Camera::Camera()
    : position_(VGet(0.0f, 10.0f, -20.0f))
    , target_(VGet(0.0f, 0.0f, 0.0f))
    , fov_(DX_PI_F / 3.0f)   // 60度
    , nearClip_(0.1f)
    , farClip_(1000.0f)
{
}

void Camera::SetPosition(VECTOR pos)
{
    position_ = pos;
}

void Camera::SetTarget(VECTOR target)
{
    target_ = target;
}

void Camera::Follow(VECTOR targetPos, VECTOR offset)
{
    // ターゲット座標にオフセットを加算してカメラ位置を決定
    position_ = VAdd(targetPos, offset);
    target_   = targetPos;
}

void Camera::SetFov(float fov)
{
    fov_ = fov;
}

void Camera::SetClip(float nearClip, float farClip)
{
    nearClip_ = nearClip;
    farClip_  = farClip;
}

void Camera::Update()
{
    // 画角とクリップ距離をDxLibに反映
    SetupCamera_Perspective(fov_);
    SetCameraNearFar(nearClip_, farClip_);

    // カメラの位置と注視点をDxLibに反映
    // Y軸が上方向の基準ベクトル
    SetCameraPositionAndTarget_UpVecY(position_, target_);
}
