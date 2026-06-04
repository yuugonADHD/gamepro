#pragma once

#include <string>
#include <variant>
#include <DxLib.h>

#include "../Collision/SphereCollider.h"
#include "../Collision/AABBCollider.h"
#include "../Collision/CapsuleCollider.h"

/**
 * @brief MV1形式の3Dモデルの読み込み・描画・アニメーションを管理するクラス
 */
class Model
{
public:
    /**
     * @brief モデルを生成
     * @return なし。
     */
    Model();

    /**
     * @brief モデルを破棄。ロード済みの場合は自動でアンロード
     * @return なし。
     */
    ~Model();

    /**
     * @brief MV1ファイルを読み込み
     * @param filePath 読み込むファイルのパス
     * @return 成功したらtrue、失敗したらfalse
     */
    bool Load(const std::string& filePath);

    /**
     * @brief モデルをアンロード
     * @return なし。
     */
    void Unload();

    /**
     * @brief モデルのワールド座標を設定
     * @param pos ワールド座標
     * @return なし。
     */
    void SetPosition(VECTOR pos);

    /**
     * @brief モデルの回転角をラジアンで設定
     * @param rot 各軸の回転角（ラジアン）
     * @return なし。
     */
    void SetRotation(VECTOR rot);

    /**
     * @brief モデルのスケールを設定
     * @param scale 各軸のスケール
     * @return なし。
     */
    void SetScale(VECTOR scale);

    /**
     * @brief モデルの状態をDxLibに反映。毎フレーム呼び出す
     * @return なし。
     */
    void Update();

    /**
     * @brief モデルを描画
     * @return なし。
     */
    void Draw();

    /**
     * @brief アニメーションを再生
     * @param animIndex 再生するアニメーションのインデックス
     * @param loop trueのときループ再生
     * @return なし。
     */
    void PlayAnimation(int animIndex, bool loop = true);

    /**
     * @brief アニメーションを停止
     * @return なし。
     */
    void StopAnimation();

    /**
     * @brief アニメーションの再生時刻を進める。毎フレーム呼び出す
     * @param deltaTime 前フレームからの経過時間（秒）
     * @return なし。
     */
    void UpdateAnimation(float deltaTime);

    /**
     * @brief 指定アニメーションの総再生時間
     * @param animIndex アニメーションのインデックス
     * @return 総再生時間（秒）
     */
    float GetAnimationTotalTime(int animIndex);

    /**
     * @brief 球コライダーを設定
     * @param col 設定するSphereCollider
     * @return なし。
     */
    void SetCollider(SphereCollider col);

    /**
     * @brief AABBコライダーを設定
     * @param col 設定するAABBCollider
     * @return なし。
     */
    void SetCollider(AABBCollider col);

    /**
     * @brief カプセルコライダーを設定
     * @param col 設定するCapsuleCollider
     * @return なし。
     */
    void SetCollider(CapsuleCollider col);

    /**
     * @brief 他のモデルとの衝突判定
     * @param other 衝突チェック対象のモデル
     * @return 衝突していたらtrue、していなければfalse
     */
    bool CheckCollision(const Model& other);

    /**
     * @brief モデルのワールド座標
     * @return ワールド座標
     */
    VECTOR GetPosition() const;

private:
    /** @brief DxLibのモデルハンドル。-1のときは未ロード */
    int modelHandle_;

    /** @brief モデルのワールド座標 */
    VECTOR position_;

    /** @brief モデルの回転角（ラジアン） */
    VECTOR rotation_;

    /** @brief モデルのスケール */
    VECTOR scale_;

    /** @brief アタッチ済みアニメーションのハンドル。-1のときは未設定 */
    int animAttachIndex_;

    /** @brief アニメーションの現在再生時刻（秒） */
    float animTime_;

    /** @brief アニメーションをループ再生するかどうか */
    bool animLoop_;

    /** @brief コライダーを持つかどうか */
    bool hasCollider_;

    /**
     * @brief コライダーの種類を表す内部型
     */
    enum class ColliderType { Sphere, AABB, Capsule };

    /** @brief 現在設定されているコライダーの種類 */
    ColliderType colliderType_;

    /** @brief コライダーのデータをvariantで保持 */
    std::variant<SphereCollider, AABBCollider, CapsuleCollider> collider_;
};
