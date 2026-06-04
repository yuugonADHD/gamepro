#pragma once

#include <string>

/**
 * @brief BGM・SEの読み込みと再生を管理するシングルトンクラス
 */
class AudioManager
{
public:
    /**
     * @brief AudioManagerのインスタンスを取得
     * @return AudioManagerの唯一のインスタンス
     */
    static AudioManager& GetInstance();

    /**
     * @brief BGMファイルを読み込み
     * @param path 読み込む音声ファイルのパス
     * @return 読み込みに成功したサウンドハンドル。失敗時は-1
     */
    int LoadBGM(const std::string& path) const;

    /**
     * @brief SEファイルを読み込み
     * @param path 読み込む音声ファイルのパス
     * @return 読み込みに成功したサウンドハンドル。失敗時は-1
     */
    int LoadSE(const std::string& path) const;

    /**
     * @brief BGMを再生
     * @param handle 再生するサウンドハンドル
     * @param loop trueならループ再生、falseなら1回再生
     * @return なし。
     */
    void PlayBGM(int handle, bool loop = true) const;

    /**
     * @brief SEを再生
     * @param handle 再生するサウンドハンドル
     * @return なし。
     */
    void PlaySE(int handle) const;

    /**
     * @brief BGMを停止
     * @param handle 停止するサウンドハンドル
     * @return なし。
     */
    void StopBGM(int handle) const;

    /**
     * @brief BGMのサウンドハンドルを解放
     * @param handle 解放するサウンドハンドル
     * @return なし。
     */
    void UnloadBGM(int handle) const;

    /**
     * @brief SEのサウンドハンドルを解放
     * @param handle 解放するサウンドハンドル
     * @return なし。
     */
    void UnloadSE(int handle) const;

private:
    /**
     * @brief AudioManagerの生成を禁止するためのコンストラクタ
     * @return なし。
     */
    AudioManager();

    /**
     * @brief AudioManagerのコピーを禁止
     * @param other コピー元オブジェクト
     * @return なし。
     */
    AudioManager(const AudioManager& other) = delete;

    /**
     * @brief AudioManagerの代入を禁止
     * @param other 代入元オブジェクト
     * @return 代入結果の参照
     */
    AudioManager& operator=(const AudioManager& other) = delete;
};
