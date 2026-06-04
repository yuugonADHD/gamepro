#pragma once

#include <DxLib.h>

/**
 * @brief カプセルコライダー。カプセルは、両端の球の集合の形状
 */
struct CapsuleCollider
{
    /** @brief カプセルの半径 */
    float radius;

    /** @brief カプセルの高さ（両端の球を除いた内部長さ） */
    float height;

    /** @brief モデル位置からのオフセット */
    VECTOR offset;

    /**
     * @brief カプセルコライダーを生成
     * @param radius 半径
     * @param height 内部長さの高さ
     */
    explicit CapsuleCollider(float radius = 0.5f, float height = 2.0f)
        : radius(radius), height(height), offset(VGet(0.0f, 0.0f, 0.0f))
    {
    }

    /**
     * @brief ワールド空間における上端の中心座標を返す
     * @param modelPos モデルのワールド座標
     * @return ワールド空間における上端の中心座標
     */
    VECTOR GetWorldTop(VECTOR modelPos) const
    {
        const VECTOR base = VAdd(modelPos, offset);
        return VGet(base.x, base.y + height * 0.5f, base.z);
    }

    /**
     * @brief ワールド空間における下端の中心座標を返す
     * @param modelPos モデルのワールド座標
     * @return ワールド空間における下端の中心座標
     */
    VECTOR GetWorldBottom(VECTOR modelPos) const
    {
        const VECTOR base = VAdd(modelPos, offset);
        return VGet(base.x, base.y - height * 0.5f, base.z);
    }
};
