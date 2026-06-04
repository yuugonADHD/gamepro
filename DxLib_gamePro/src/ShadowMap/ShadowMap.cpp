#include "ShadowMap.h"

ShadowMap::ShadowMap()
    : shadowMapHandle_(-1)
{
}

ShadowMap::~ShadowMap()
{
    Finalize();
}

bool ShadowMap::Init(int width, int height)
{
    shadowMapHandle_ = MakeShadowMap(width, height);
    return shadowMapHandle_ != -1;
}

void ShadowMap::Finalize()
{
    if (shadowMapHandle_ != -1)
    {
        DeleteShadowMap(shadowMapHandle_);
        shadowMapHandle_ = -1;
    }
}

void ShadowMap::SetLightDirection(VECTOR dir)
{
    if (shadowMapHandle_ == -1)
    {
        return;
    }

    SetShadowMapLightDirection(shadowMapHandle_, dir);
}

void ShadowMap::SetArea(VECTOR center, float range)
{
    if (shadowMapHandle_ == -1)
    {
        return;
    }

    // ライトが影響するワールド空間上の範囲を設定
    SetShadowMapDrawArea(shadowMapHandle_,
        VGet(center.x - range, center.y - range, center.z - range),
        VGet(center.x + range, center.y + range, center.z + range));
}

void ShadowMap::BeginDraw()
{
    if (shadowMapHandle_ == -1)
    {
        return;
    }

    // シャドウマップへの描画モードに切り替え
    ShadowMap_DrawSetup(shadowMapHandle_);
}

void ShadowMap::EndDraw()
{
    if (shadowMapHandle_ == -1)
    {
        return;
    }

    // シャドウマップへの描画モードを終了
    ShadowMap_DrawEnd();
}

void ShadowMap::Apply()
{
    if (shadowMapHandle_ == -1)
    {
        return;
    }

    // シャドウマップを有効化してシーン描画時に影が落ちるように
    SetUseShadowMap(0, shadowMapHandle_);
}

void ShadowMap::Clear()
{
    // -1を渡すことでスロット0のシャドウマップの使用を解除
    SetUseShadowMap(0, -1);
}
