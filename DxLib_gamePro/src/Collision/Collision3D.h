#pragma once

#include "SphereCollider.h"
#include "AABBCollider.h"
#include "CapsuleCollider.h"

#include <DxLib.h>

/**
 * @brief 3Dコライダー間の衝突判定を担うユーティリティクラス
 *
 * すべてのメソッドはstatic。インスタンスを生成せずに使用可能
 */
class Collision3D
{
public:
    /**
     * @brief 球と球の衝突判定
     * @param a    球コライダーA
     * @param posA コライダーAのモデル座標
     * @param b    球コライダーB
     * @param posB コライダーBのモデル座標
     * @return 衝突していたらtrue、していなければfalse
     */
    static bool SphereToSphere(const SphereCollider& a, VECTOR posA,
                                const SphereCollider& b, VECTOR posB);

    /**
     * @brief AABBとAABBの衝突判定
     * @param a    AABBコライダーA
     * @param posA コライダーAのモデル座標
     * @param b    AABBコライダーB
     * @param posB コライダーBのモデル座標
     * @return 衝突していたらtrue、していなければfalse
     */
    static bool AABBToAABB(const AABBCollider& a, VECTOR posA,
                            const AABBCollider& b, VECTOR posB);

    /**
     * @brief 球とAABBの衝突判定
     * @param sphere 球コライダー
     * @param posS   球のモデル座標
     * @param aabb   AABBコライダー
     * @param posA   AABBのモデル座標
     * @return 衝突していたらtrue、していなければfalse
     */
    static bool SphereToAABB(const SphereCollider& sphere, VECTOR posS,
                              const AABBCollider&   aabb,   VECTOR posA);

    /**
     * @brief カプセルとカプセルの衝突判定
     * @param a    カプセルコライダーA
     * @param posA コライダーAのモデル座標
     * @param b    カプセルコライダーB
     * @param posB コライダーBのモデル座標
     * @return 衝突していたらtrue、していなければfalse
     */
    static bool CapsuleToCapsule(const CapsuleCollider& a, VECTOR posA,
                                  const CapsuleCollider& b, VECTOR posB);

    /**
     * @brief 球とカプセルの衝突判定
     * @param sphere  球コライダー
     * @param posS    球のモデル座標
     * @param capsule カプセルコライダー
     * @param posC    カプセルのモデル座標
     * @return 衝突していたらtrue、していなければfalse
     */
    static bool SphereToCapsule(const SphereCollider&  sphere,  VECTOR posS,
                                 const CapsuleCollider& capsule, VECTOR posC);

    /**
     * @brief AABBとカプセルの衝突判定
     * @param aabb    AABBコライダー
     * @param posA    AABBのモデル座標
     * @param capsule カプセルコライダー
     * @param posC    カプセルのモデル座標
     * @return 衝突していたらtrue、していなければfalse
     */
    static bool AABBToCapsule(const AABBCollider&    aabb,    VECTOR posA,
                               const CapsuleCollider& capsule, VECTOR posC);

private:
    // インスタンス化禁止
    Collision3D() = delete;
};
