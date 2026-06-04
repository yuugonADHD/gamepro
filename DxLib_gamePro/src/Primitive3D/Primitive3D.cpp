#include "Primitive3D.h"

void Primitive3D::DrawSphere(VECTOR center, float radius, COLOR_U8 color, bool fill, bool lighting)
{
    // lighting = false のときはライティングを一時無効化
    if (!lighting) SetUseLighting(FALSE);
    DrawSphere3D(center, radius,
        16,  // 分割数（多いほど滑らか）
        GetColor(color.r, color.g, color.b),
        GetColor(color.r, color.g, color.b),
        fill ? TRUE : FALSE);
    if (!lighting) SetUseLighting(TRUE);
}

void Primitive3D::DrawBox(VECTOR min, VECTOR max, COLOR_U8 color, bool fill, bool lighting)
{
    // lighting = false のときはライティングを一時無効化
    if (!lighting) SetUseLighting(FALSE);
    const unsigned int col = GetColor(color.r, color.g, color.b);
    DrawCube3D(min, max, col, col, fill ? TRUE : FALSE);
    if (!lighting) SetUseLighting(TRUE);
}

void Primitive3D::DrawLine(VECTOR start, VECTOR end, COLOR_U8 color, float width, bool lighting)
{
    // lighting = false のときはライティングを一時無効化
    if (!lighting) SetUseLighting(FALSE);
    DrawLine3D(start, end, GetColor(color.r, color.g, color.b));
    (void)width; // DxLib の DrawLine3D は太さ指定が無いため未使用
    if (!lighting) SetUseLighting(TRUE);
}

void Primitive3D::DrawCapsule(VECTOR start, VECTOR end, float radius, COLOR_U8 color, bool fill, bool lighting)
{
    // lighting = false のときはライティングを一時無効化
    if (!lighting) SetUseLighting(FALSE);
    DrawCapsule3D(start, end, radius,
        16,  // 分割数
        GetColor(color.r, color.g, color.b),
        GetColor(color.r, color.g, color.b),
        fill ? TRUE : FALSE);
    if (!lighting) SetUseLighting(TRUE);
}
