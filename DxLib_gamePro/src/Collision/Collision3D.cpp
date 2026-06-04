#include "Collision3D.h"

#include <cmath>
#include <algorithm>

// ---- 内部ヘルパー -------------------------------------------------------

// 2点間の距離の2乗（sqrt を省いてコスト削減）
static float DistanceSq(VECTOR a, VECTOR b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    const float dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

// 値を [lo, hi] にクランプ
static float Clampf(float v, float lo, float hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// ベクトルのドット積
static float Dot(VECTOR a, VECTOR b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 線分 [p0, p1] 上で点 pt に最も近い点
static VECTOR ClosestPointOnSegment(VECTOR p0, VECTOR p1, VECTOR pt)
{
    const VECTOR d  = VSub(p1, p0);
    const float lenSq = Dot(d, d);

    if (lenSq < 1e-10f)
    {
        return p0; // 線分が点に縮退している場合
    }

    const float t = Clampf(Dot(VSub(pt, p0), d) / lenSq, 0.0f, 1.0f);
    return VAdd(p0, VScale(d, t));
}

// -------------------------------------------------------------------------

bool Collision3D::SphereToSphere(const SphereCollider& a, VECTOR posA,
                                  const SphereCollider& b, VECTOR posB)
{
    // 2つの球の中心間の距離が半径の合計より小さければ衝突してい
    const float radiusSum = a.radius + b.radius;
    return DistanceSq(a.GetWorldCenter(posA), b.GetWorldCenter(posB))
           < radiusSum * radiusSum;
}

bool Collision3D::AABBToAABB(const AABBCollider& a, VECTOR posA,
                               const AABBCollider& b, VECTOR posB)
{
    // 各軸で min/max が重なっているかを確認します（AABB判定）。
    const VECTOR minA = a.GetWorldMin(posA);
    const VECTOR maxA = a.GetWorldMax(posA);
    const VECTOR minB = b.GetWorldMin(posB);
    const VECTOR maxB = b.GetWorldMax(posB);

    return minA.x < maxB.x && maxA.x > minB.x
        && minA.y < maxB.y && maxA.y > minB.y
        && minA.z < maxB.z && maxA.z > minB.z;
}

bool Collision3D::SphereToAABB(const SphereCollider& sphere, VECTOR posS,
                                 const AABBCollider&   aabb,   VECTOR posA)
{
    // AABBの外面上で球の中心に最も近い点を求め、距離が半径以内なら衝突
    const VECTOR center = sphere.GetWorldCenter(posS);
    const VECTOR minB   = aabb.GetWorldMin(posA);
    const VECTOR maxB   = aabb.GetWorldMax(posA);

    const float cx = Clampf(center.x, minB.x, maxB.x);
    const float cy = Clampf(center.y, minB.y, maxB.y);
    const float cz = Clampf(center.z, minB.z, maxB.z);

    const VECTOR closest = VGet(cx, cy, cz);
    return DistanceSq(center, closest) < sphere.radius * sphere.radius;
}

bool Collision3D::CapsuleToCapsule(const CapsuleCollider& a, VECTOR posA,
                                    const CapsuleCollider& b, VECTOR posB)
{
    // 2つのカプセルの軸線分間の最短距離が半径の合計より小さければ衝突
    const VECTOR a0 = a.GetWorldBottom(posA);
    const VECTOR a1 = a.GetWorldTop(posA);
    const VECTOR b0 = b.GetWorldBottom(posB);
    const VECTOR b1 = b.GetWorldTop(posB);

    // 線分A上のtと線分B上のsを交互に収束させて最近点を求める
    VECTOR closestA = ClosestPointOnSegment(a0, a1, b0);
    VECTOR closestB = ClosestPointOnSegment(b0, b1, closestA);
    closestA = ClosestPointOnSegment(a0, a1, closestB);

    const float radiusSum = a.radius + b.radius;
    return DistanceSq(closestA, closestB) < radiusSum * radiusSum;
}

bool Collision3D::SphereToCapsule(const SphereCollider&  sphere,  VECTOR posS,
                                   const CapsuleCollider& capsule, VECTOR posC)
{
    // カプセルの軸線分上で球の中心に最も近い点を求め、距離が半径の合計以内なら衝突
    const VECTOR center  = sphere.GetWorldCenter(posS);
    const VECTOR capBot  = capsule.GetWorldBottom(posC);
    const VECTOR capTop  = capsule.GetWorldTop(posC);
    const VECTOR closest = ClosestPointOnSegment(capBot, capTop, center);

    const float radiusSum = sphere.radius + capsule.radius;
    return DistanceSq(center, closest) < radiusSum * radiusSum;
}

bool Collision3D::AABBToCapsule(const AABBCollider&    aabb,    VECTOR posA,
                                 const CapsuleCollider& capsule, VECTOR posC)
{
    // カプセルの軸線分上でAABBの中心に最も近い点を求め、
    // その点を中心とする球とAABBの判定で近似
    const VECTOR capBot     = capsule.GetWorldBottom(posC);
    const VECTOR capTop     = capsule.GetWorldTop(posC);
    const VECTOR aabbCenter = VAdd(posA, aabb.offset);
    const VECTOR closest    = ClosestPointOnSegment(capBot, capTop, aabbCenter);

    return SphereToAABB(SphereCollider(capsule.radius), closest, aabb, posA);
}
