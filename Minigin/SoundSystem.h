#pragma once

#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <SDL_mixer.h>
#include <functional>

namespace dae
{
    enum class SoundType { SoundEffect, Music };

    struct SoundEvent
    {
        SoundType type;
        std::string fileName;
        int volume;
        int loops;
    };

    class SoundSystem
    {
    public:
        virtual void PlaySoundEffect(const std::string& fileName, int volume, int loops) = 0;
        virtual void PlayMusic(const std::string& fileName, int volume, int loops) = 0;
        virtual void MuteAll() = 0;
        virtual void UnmuteAll() = 0;
        virtual void UnloadMusic(const std::string& fileName) = 0;
        virtual void UnloadSoundEffect(const std::string& fileName) = 0;
        virtual ~SoundSystem() = default;
    };

    class SDLSoundSystem : public SoundSystem
    {
    public:
        SDLSoundSystem();
        ~SDLSoundSystem();

        void PlaySoundEffect(const std::string& fileName, int volume, int loops) override;
        void PlayMusic(const std::string& fileName, int volume, int loops) override;
        void MuteAll() override;
        void UnmuteAll() override;
        void StopMusic();
        void UnloadMusic(const std::string& fileName) override;
        void UnloadSoundEffect(const std::string& fileName) override;

        void PreloadSoundEffect(const std::string& fileName);
        void PreloadMusic(const std::string& fileName);

    private:
        void SoundLoaderThread();
        bool IsEventAvailableOrShouldQuit() const;

        std::unique_ptr<std::thread> m_pThread;
        std::queue<SoundEvent> m_EventQueue;
        std::mutex m_QueueMutex;
        std::condition_variable m_ConditionVariable;
        bool m_ShouldQuit = false;
        class SDL_MixerImpl;
        std::unique_ptr<SDL_MixerImpl> m_pSDL_MixerImpl;

        std::map<std::string, Mix_Chunk*> m_PreloadedSoundEffects;
        std::map<std::string, Mix_Music*> m_PreloadedMusic;
    };
}