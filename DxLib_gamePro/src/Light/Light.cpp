#include "Light.h"

Light::Light() = default;

void Light::Init()
{
    // ライトを平行光源として設定し、デフォルトの方向・色を適用
    ChangeLightTypeDir(VGet(-1.0f, -2.0f, -1.0f));

    // 拡散光: ほぼ白
    SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 1.0f));

    // 環境光: 暗部が真っ黒にならないよう少し明るめのグレーを使用
    SetLightAmbColor(GetColorF(0.3f, 0.3f, 0.3f, 1.0f));

    // 鏡面反射光: オフ（白 × 0）
    SetLightSpcColor(GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
}

void Light::SetDirection(VECTOR dir)
{
    ChangeLightTypeDir(dir);
}

void Light::SetDiffuse(COLOR_F color)
{
    SetLightDifColor(color);
}

void Light::SetAmbient(COLOR_F color)
{
    SetLightAmbColor(color);
}

void Light::SetSpecular(COLOR_F color)
{
    SetLightSpcColor(color);
}
