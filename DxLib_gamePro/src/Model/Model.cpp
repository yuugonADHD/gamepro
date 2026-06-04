#include "Model.h"

#include "../Collision/Collision3D.h"

#include <cmath>

/**
 * @brief モデルを生成し、デフォルト値を設定
 */
Model::Model()
    : modelHandle_(-1)
    , position_(VGet(0.0f, 0.0f, 0.0f))
    , rotation_(VGet(0.0f, 0.0f, 0.0f))
    , scale_(VGet(1.0f, 1.0f, 1.0f))
    , animAttachIndex_(-1)
    , animTime_(0.0f)
    , animLoop_(true)
    , hasCollider_(false)
    , colliderType_(ColliderType::Sphere)
    , collider_(SphereCollider())
{
}

/**
 * @brief モデルを破棄。ロード済みの場合は自動でアンロード
 */
Model::~Model()
{
    Unload();
}

bool Model::Load(const std::string& filePath)
{
    modelHandle_ = MV1LoadModel(filePath.c_str());
    return modelHandle_ != -1;
}

void Model::Unload()
{
    if (modelHandle_ != -1)
    {
        MV1DeleteModel(modelHandle_);
        modelHandle_ = -1;
    }
}

void Model::SetPosition(VECTOR pos)
{
    position_ = pos;
}

void Model::SetRotation(VECTOR rot)
{
    rotation_ = rot;
}

void Model::SetScale(VECTOR scale)
{
    scale_ = scale;
}

void Model::Update()
{
    if (modelHandle_ == -1)
    {
        return;
    }

    // 位置・回転・スケールをDxLibのモデルに反映
    MV1SetPosition(modelHandle_, position_);
    MV1SetRotationXYZ(modelHandle_, rotation_);
    MV1SetScale(modelHandle_, scale_);
}

void Model::Draw()
{
    if (modelHandle_ == -1)
    {
        return;
    }

    MV1DrawModel(modelHandle_);
}

void Model::PlayAnimation(int animIndex, bool loop)
{
    if (modelHandle_ == -1)
    {
        return;
    }

    // 既存のアニメーションがあればデタッチしてから新しくアタッチ
    if (animAttachIndex_ != -1)
    {
        MV1DetachAnim(modelHandle_, animAttachIndex_);
    }

    animAttachIndex_ = MV1AttachAnim(modelHandle_, animIndex);
    animTime_ = 0.0f;
    animLoop_ = loop;
}

void Model::StopAnimation()
{
    if (modelHandle_ == -1 || animAttachIndex_ == -1)
    {
        return;
    }

    MV1DetachAnim(modelHandle_, animAttachIndex_);
    animAttachIndex_ = -1;
    animTime_ = 0.0f;
}

void Model::UpdateAnimation(float deltaTime)
{
    if (modelHandle_ == -1 || animAttachIndex_ == -1)
    {
        return;
    }

    const float totalTime = MV1GetAttachAnimTotalTime(modelHandle_, animAttachIndex_);

    // アニメーション時刻を deltaTime 分だけ進め
    animTime_ += deltaTime;

    if (animLoop_)
    {
        // ループ再生: 総再生時間を超えたら先頭に戻
        if (animTime_ >= totalTime)
        {
            animTime_ = std::fmod(animTime_, totalTime);
        }
    }
    else
    {
        // 非ループ再生: 終端でクランプ
        if (animTime_ > totalTime)
        {
            animTime_ = totalTime;
        }
    }

    MV1SetAttachAnimTime(modelHandle_, animAttachIndex_, animTime_);
}

float Model::GetAnimationTotalTime(int animIndex)
{
    if (modelHandle_ == -1)
    {
        return 0.0f;
    }

    // 一時的にアタッチして総再生時間を取得
    const int tempAttach = MV1AttachAnim(modelHandle_, animIndex);
    const float totalTime = MV1GetAttachAnimTotalTime(modelHandle_, tempAttach);
    MV1DetachAnim(modelHandle_, tempAttach);
    return totalTime;
}

void Model::SetCollider(SphereCollider col)
{
    collider_     = col;
    colliderType_ = ColliderType::Sphere;
    hasCollider_  = true;
}

void Model::SetCollider(AABBCollider col)
{
    collider_     = col;
    colliderType_ = ColliderType::AABB;
    hasCollider_  = true;
}

void Model::SetCollider(CapsuleCollider col)
{
    collider_     = col;
    colliderType_ = ColliderType::Capsule;
    hasCollider_  = true;
}

bool Model::CheckCollision(const Model& other)
{
    // どちらかがコライダーを持っていなければ衝突なしと
    if (!hasCollider_ || !other.hasCollider_)
    {
        return false;
    }

    // 自身が球コライダーの場合
    if (colliderType_ == ColliderType::Sphere)
    {
        const SphereCollider& a = std::get<SphereCollider>(collider_);

        if (other.colliderType_ == ColliderType::Sphere)
        {
            return Collision3D::SphereToSphere(a, position_,
                std::get<SphereCollider>(other.collider_), other.position_);
        }
        if (other.colliderType_ == ColliderType::AABB)
        {
            return Collision3D::SphereToAABB(a, position_,
                std::get<AABBCollider>(other.collider_), other.position_);
        }
        if (other.colliderType_ == ColliderType::Capsule)
        {
            return Collision3D::SphereToCapsule(a, position_,
                std::get<CapsuleCollider>(other.collider_), other.position_);
        }
    }

    // 自身がAABBコライダーの場合
    if (colliderType_ == ColliderType::AABB)
    {
        const AABBCollider& a = std::get<AABBCollider>(collider_);

        if (other.colliderType_ == ColliderType::AABB)
        {
            return Collision3D::AABBToAABB(a, position_,
                std::get<AABBCollider>(other.collider_), other.position_);
        }
        if (other.colliderType_ == ColliderType::Sphere)
        {
            return Collision3D::SphereToAABB(
                std::get<SphereCollider>(other.collider_), other.position_, a, position_);
        }
        if (other.colliderType_ == ColliderType::Capsule)
        {
            return Collision3D::AABBToCapsule(a, position_,
                std::get<CapsuleCollider>(other.collider_), other.position_);
        }
    }

    // 自身がカプセルコライダーの場合
    if (colliderType_ == ColliderType::Capsule)
    {
        const CapsuleCollider& a = std::get<CapsuleCollider>(collider_);

        if (other.colliderType_ == ColliderType::Capsule)
        {
            return Collision3D::CapsuleToCapsule(a, position_,
                std::get<CapsuleCollider>(other.collider_), other.position_);
        }
        if (other.colliderType_ == ColliderType::Sphere)
        {
            return Collision3D::SphereToCapsule(
                std::get<SphereCollider>(other.collider_), other.position_, a, position_);
        }
    }

    return false;
}

VECTOR Model::GetPosition() const
{
    return position_;
}
