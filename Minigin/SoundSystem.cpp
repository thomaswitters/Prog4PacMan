#include "SoundSystem.h"

namespace dae
{
    class SDLSoundSystem::SDL_MixerImpl
    {
    public:
        SDL_MixerImpl()
        {
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                std::cerr << "SDL_Mixer couldn't initialize: " << Mix_GetError() << std::endl;
            }
        }

        ~SDL_MixerImpl()
        {
            for (auto& sound : m_SoundEffectsMap)
            {
                Mix_FreeChunk(sound.second);
            }
            for (auto& music : m_MusicMap)
            {
                Mix_FreeMusic(music.second);
            }
            Mix_CloseAudio();
        }

        void PlaySoundEffect(const std::string& fileName, int volume, int loops)
        {
            Mix_Chunk* chunk = LoadSoundEffect(fileName);
            if (chunk)
            {
                volume = (MIX_MAX_VOLUME * volume) / 100;
                Mix_VolumeChunk(chunk, volume);
                Mix_PlayChannel(-1, chunk, loops);
            }
        }

        void PlayMusic(const std::string& fileName, int volume, int loops)
        {
            Mix_Music* music = LoadMusic(fileName);
            if (music)
            {
                volume = (MIX_MAX_VOLUME * volume) / 100;
                Mix_VolumeMusic(volume);
                if (loops == -1)
                    Mix_PlayMusic(music, -1);
                else
                    Mix_PlayMusic(music, loops);
            }
        }

        Mix_Chunk* LoadSoundEffect(const std::string& fileName)
        {
            Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());
            if (!chunk)
            {
                std::cerr << "Failed to load sound effect '" << fileName << "': " << Mix_GetError() << std::endl;
            }
            return chunk;
        }

        Mix_Music* LoadMusic(const std::string& fileName)
        {
            Mix_Music* music = Mix_LoadMUS(fileName.c_str());
            if (!music)
            {
                std::cerr << "Failed to load music '" << fileName << "': " << Mix_GetError() << std::endl;
            }
            return music;
        }

    private:
        std::map<std::string, Mix_Chunk*> m_SoundEffectsMap;
        std::map<std::string, Mix_Music*> m_MusicMap;
    };

    SDLSoundSystem::SDLSoundSystem()
        : m_pSDL_MixerImpl(std::make_unique<SDL_MixerImpl>())
    {
        PreloadSoundEffect("../Data/Sounds/munch_1.wav");
        PreloadSoundEffect("../Data/Sounds/munch_2.wav");
        PreloadSoundEffect("../Data/Sounds/eat_ghost.wav");
        PreloadSoundEffect("../Data/Sounds/power_pellet.wav");
        PreloadMusic("../Data/Sounds/1-03. PAC-MAN NEVA PAX!!.mp3");

        m_pThread = std::make_unique<std::thread>(&SDLSoundSystem::SoundLoaderThread, this);
    }

    SDLSoundSystem::~SDLSoundSystem()
    {
        for (auto& sound : m_PreloadedSoundEffects)
        {
            Mix_FreeChunk(sound.second);
        }

        for (auto& music : m_PreloadedMusic)
        {
            Mix_FreeMusic(music.second);
        }

        {
            std::lock_guard<std::mutex> lock(m_QueueMutex);
            m_ShouldQuit = true;
        }
        m_ConditionVariable.notify_all();
        if (m_pThread && m_pThread->joinable())
        {
            m_pThread->join();
        }
    }

    void SDLSoundSystem::PreloadSoundEffect(const std::string& fileName)
    {
        if (m_PreloadedSoundEffects.find(fileName) != m_PreloadedSoundEffects.end())
            return;

        Mix_Chunk* chunk = m_pSDL_MixerImpl->LoadSoundEffect(fileName);
        if (chunk)
        {
            m_PreloadedSoundEffects[fileName] = chunk;
        }
    }

    void SDLSoundSystem::PreloadMusic(const std::string& fileName)
    {
        if (m_PreloadedMusic.find(fileName) != m_PreloadedMusic.end())
            return;

        Mix_Music* music = m_pSDL_MixerImpl->LoadMusic(fileName);
        if (music)
        {
            m_PreloadedMusic[fileName] = music;
        }
    }

    void SDLSoundSystem::PlaySoundEffect(const std::string& fileName, int volume, int loops)
    {
        auto it = m_PreloadedSoundEffects.find(fileName);
        if (it != m_PreloadedSoundEffects.end())
        {
            Mix_Chunk* chunk = it->second;
            volume = (MIX_MAX_VOLUME * volume) / 100;
            Mix_VolumeChunk(chunk, volume);
            Mix_PlayChannel(-1, chunk, loops);
        }
        else
        {
            {
                std::lock_guard<std::mutex> lock(m_QueueMutex);
                m_EventQueue.push({ SoundType::SoundEffect, fileName, volume, loops });
            }
            m_ConditionVariable.notify_all();
        }
    }

    void SDLSoundSystem::PlayMusic(const std::string& fileName, int volume, int loops)
    {
        auto it = m_PreloadedMusic.find(fileName);
        if (it != m_PreloadedMusic.end())
        {
            Mix_Music* music = it->second;
            volume = (MIX_MAX_VOLUME * volume) / 100;
            Mix_VolumeMusic(volume);
            if (loops == -1)
                Mix_PlayMusic(music, -1);
            else
                Mix_PlayMusic(music, loops);
        }
        else
        {
            {
                std::lock_guard<std::mutex> lock(m_QueueMutex);
                m_EventQueue.push({ SoundType::Music, fileName, volume, loops });
            }
            m_ConditionVariable.notify_all();
        }
    }

    void SDLSoundSystem::SoundLoaderThread()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_ConditionVariable.wait(lock, std::bind(&SDLSoundSystem::IsEventAvailableOrShouldQuit, this));

            if (m_ShouldQuit)
            {
                break;
            }

            SoundEvent event = m_EventQueue.front();
            m_EventQueue.pop();
            lock.unlock();

            if (event.type == SoundType::SoundEffect)
            {
                m_pSDL_MixerImpl->PlaySoundEffect(event.fileName, event.volume, event.loops);
            }
            else if (event.type == SoundType::Music)
            {
                m_pSDL_MixerImpl->PlayMusic(event.fileName, event.volume, event.loops);
            }
        }
    }

    void SDLSoundSystem::MuteAll()
    {
        Mix_Volume(-1, 0);
        Mix_VolumeMusic(0);
    }

    void SDLSoundSystem::UnmuteAll()
    {
        Mix_VolumeMusic(MIX_MAX_VOLUME);
        Mix_Volume(-1, MIX_MAX_VOLUME);

        for (const auto& sound : m_PreloadedSoundEffects)
        {
            Mix_VolumeChunk(sound.second, MIX_MAX_VOLUME);
        }
    }

    bool SDLSoundSystem::IsEventAvailableOrShouldQuit() const
    {
        return !m_EventQueue.empty() || m_ShouldQuit;
    }

    void SDLSoundSystem::StopMusic()
    {
        Mix_HaltMusic();
    }

    void SDLSoundSystem::UnloadMusic(const std::string& fileName)
    {
        auto it = m_PreloadedMusic.find(fileName);
        if (it != m_PreloadedMusic.end())
        {
            Mix_FreeMusic(it->second);
            m_PreloadedMusic.erase(it);
        }
    }
    void SDLSoundSystem::UnloadSoundEffect(const std::string& fileName)
    {
        auto it = m_PreloadedSoundEffects.find(fileName);
        if (it != m_PreloadedSoundEffects.end())
        {
            Mix_FreeChunk(it->second);
            m_PreloadedSoundEffects.erase(it);
        }
    }
}