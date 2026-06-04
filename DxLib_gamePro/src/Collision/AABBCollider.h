#pragma once

#include <DxLib.h>

/**
 * @brief 軸並行境界ボックス（AABB）コライダー
 */
struct AABBCollider
{
    /** @brief ボックスの各軸の半辺長 */
    VECTOR halfExtents;

    /** @brief モデル位置からのオフセット */
    VECTOR offset;

    /**
     * @brief AABBコライダーを生成
     * @param halfExtents 各軸の半辺長
     */
    explicit AABBCollider(VECTOR halfExtents = VGet(1.0f, 1.0f, 1.0f))
        : halfExtents(halfExtents), offset(VGet(0.0f, 0.0f, 0.0f))
    {
    }

    /**
     * @brief ワールド空間における最小座標を返す
     * @param modelPos モデルのワールド座標
     * @return ワールド空間における最小座標
     */
    VECTOR GetWorldMin(VECTOR modelPos) const
    {
        const VECTOR center = VAdd(modelPos, offset);
        return VSub(center, halfExtents);
    }

    /**
     * @brief ワールド空間における最大座標を返す
     * @param modelPos モデルのワールド座標
     * @return ワールド空間における最大座標
     */
    VECTOR GetWorldMax(VECTOR modelPos) const
    {
        const VECTOR center = VAdd(modelPos, offset);
        return VAdd(center, halfExtents);
    }
};
