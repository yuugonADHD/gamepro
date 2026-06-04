#pragma once

#include <DxLib.h>

/**
 * @brief 球コライダー
 */
struct SphereCollider
{
    /** @brief 球の半径 */
    float radius;

    /** @brief モデル位置からのオフセット */
    VECTOR offset;

    /**
     * @brief 球コライダーを生成
     * @param radius 球の半径
     */
    explicit SphereCollider(float radius = 1.0f)
        : radius(radius), offset(VGet(0.0f, 0.0f, 0.0f))
    {
    }

    /**
     * @brief オフセットを設定
     * @param offset モデル位置からのオフセット
     * @return なし。
     */
    void SetOffset(VECTOR offset_)
    {
        offset = offset_;
    }

    /**
     * @brief ワールド空間における球の中心座標を返す
     * @param modelPos モデルのワールド座標
     * @return ワールド空間における球の中心座標
     */
    VECTOR GetWorldCenter(VECTOR modelPos) const
    {
        return VAdd(modelPos, offset);
    }
};
