#include "AudioManager.h"

#include <DxLib.h>

/**
 * @brief AudioManagerの生成を禁止するためのコンストラクタ
 * @return なし。
 */
AudioManager::AudioManager() = default;

/**
 * @brief AudioManagerのインスタンスを取得
 * @return AudioManagerの唯一のインスタンス
 */
AudioManager& AudioManager::GetInstance()
{
    static AudioManager instance;
    return instance;
}

/**
 * @brief BGMファイルを読み込み
 * @param path 読み込む音声ファイルのパス
 * @return 読み込みに成功したサウンドハンドル、失敗時は-1
 */
int AudioManager::LoadBGM(const std::string& path) const
{
    return LoadSoundMem(path.c_str());
}

/**
 * @brief SEファイルを読み込み
 * @param path 読み込む音声ファイルのパス
 * @return 読み込みに成功したサウンドハンドル、失敗時は-1
 */
int AudioManager::LoadSE(const std::string& path) const
{
    return LoadSoundMem(path.c_str());
}

/**
 * @brief BGMを再生
 * @param handle 再生するサウンドハンドル
 * @param loop trueならループ再生、falseなら1回再生
 * @return なし。
 */
void AudioManager::PlayBGM(const int handle, const bool loop) const
{
    if (handle < 0)
    {
        return;
    }

    const int playType = loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
    PlaySoundMem(handle, playType);
}

/**
 * @brief SEを再生
 * @param handle 再生するサウンドハンドル
 * @return なし。
 */
void AudioManager::PlaySE(const int handle) const
{
    if (handle < 0)
    {
        return;
    }

    PlaySoundMem(handle, DX_PLAYTYPE_BACK);
}

/**
 * @brief BGMを停止
 * @param handle 停止するサウンドハンドル
 * @return なし。
 */
void AudioManager::StopBGM(const int handle) const
{
    if (handle < 0)
    {
        return;
    }

    StopSoundMem(handle);
}

/**
 * @brief BGMのサウンドハンドルを解放
 * @param handle 解放するサウンドハンドル
 * @return なし。
 */
void AudioManager::UnloadBGM(const int handle) const
{
    if (handle >= 0)
    {
        DeleteSoundMem(handle);
    }
}

/**
 * @brief SEのサウンドハンドルを解放
 * @param handle 解放するサウンドハンドル
 * @return なし。
 */
void AudioManager::UnloadSE(const int handle) const
{
    if (handle >= 0)
    {
        DeleteSoundMem(handle);
    }
}
