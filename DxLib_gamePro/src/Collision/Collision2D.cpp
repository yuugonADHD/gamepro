#include "Collision2D.h"

#include <cmath>

bool Collision2D::SquareToSquare(float ax, float ay, int aSize,
                                  float bx, float by, int bSize)
{
    // 各辺のオーバーラップ確認。正方形はRectToRectの特殊ケース
    return ax < bx + bSize  && ax + aSize > bx
        && ay < by + bSize  && ay + aSize > by;
}

bool Collision2D::RectToRect(float ax, float ay, int aWidth, int aHeight,
                              float bx, float by, int bWidth, int bHeight)
{
    // 全軸でオーバーラップしていれば衝突
    return ax < bx + bWidth  && ax + aWidth  > bx
        && ay < by + bHeight && ay + aHeight > by;
}

bool Collision2D::CircleToCircle(float ax, float ay, float aRadius,
                                  float bx, float by, float bRadius)
{
    // 2点間の距離の二乗と半径の和の二乗を比較 (sqrt不要でコスト削減)
    const float dx        = ax - bx;
    const float dy        = ay - by;
    const float radiusSum = aRadius + bRadius;
    return dx * dx + dy * dy < radiusSum * radiusSum;
}
