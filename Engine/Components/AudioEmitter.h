#pragma once
#include "GameObject.h"
#include "Audio.h"

/**
 * @brief The AudioEmitter class represents a component that **owns** a Sound 
 * and does manipulations on it.
 */
class AudioEmitter : public GameComponent {
public:
    /**
     * @brief GetType static function for simple component reflection.
     * @return The type of the AudioEmitter component.
     */
    static std::string GetType() { return "AudioEmitter"; };

    static bool ConditionHasAudioEmitter(GameObject* obj);

    /**
     * @brief AudioEmitter constructor.
     */
    AudioEmitter();

    /**
     * @brief AudioEmitter constructor with attached GameObject.
     * @param attachedObj The GameObject to attach the AudioEmitter to.
     */
    AudioEmitter(GameObject* obj);

    /**
     * @brief AudioEmitter destructor.
     */
    ~AudioEmitter();

    /**
     * @brief OnAdd responsible for adding the obejct to the cache for listener.
     */
    void OnAdd() override;

    /**
     * @brief OnDelete responsible for deleting the obejct from the cache for listener.
     */
    void OnDelete() override;

    /**
     * @brief SetSound sets the sound path for the AudioEmitter.
     * @param path The path of the sound file.
     * @note all SetSound methods clean the sound that is already played 
     * and push another Sound to the asset container. Hence the **ownership**.
     */
    void SetSound(const std::string& path);

    /**
     * @brief SetSound sets the sound for the AudioEmitter.
     * @param snd The sound object.
     */
    void SetSound(const Sound* snd);

    /**
     * @brief SetVolume sets sound volume.
     * @param v the volume value, should be between 0 et 100.
     */
    void SetVolume(float v);

    /**
     * @brief Plays the sound. 
     */
    void Play();

    /**
     * @brief Stop the sound. 
     */
    void Stop();

    void Pause();

    void Resume();

    /**
     * @brief SetFrequency sets sound frequency.
     * @param v the frequency value, should be between 0 et 2.
     */
    void SetFrequency(float v);

    /**
     * @brief SetLooping sets if the the sound should loop.
     * @param v The looping value.
     * @note This function has no effect after the sound has been loaded, if you want to set looping
     * value you should do it before that.
     */
    void SetLooping(bool v);

    /**
     * @brief StopIfHasFinished Stops the sound if it has finished automatically called
     * by audio listener.
     * @param snd The sound object.
     */
    void StopIfHasFinished();

    /**
     * @brief sound The Sound of the AudioEmitter.
     */
    Sound* sound = nullptr;

    /**
     * @brief volume The volume of the AudioEmitter. Should be clamped from 0.0 to 1.0.
     */
    int volume = 100;

    /**
     * @brief frequency The frequency of the sound.
     */
    float frequency = 1.0f;

    /**
     * @brief isLooping Indicates whether the AudioEmitter is looping or not.
     */
    bool isLooping = false;
};

//TODO::Document
class MultiAudioEmitter : public GameComponent {
    public:
        std::unordered_map<GameObject*, AudioEmitter*> _container;

        static std::string GetType() { return "MultiAudioEmitter";}
        inline MultiAudioEmitter() {}
        inline MultiAudioEmitter(GameObject* obj) { attachedObject = obj;}
        ~MultiAudioEmitter();

        GameObject* AddEmitter();
        void  DeleteEmitter(GameObject*);
        GameObject* GetEmitterIf(bool(*)(GameObject*));
        GameObject* GetEmitterAudioIf(bool(*)(GameObject*));
        AudioEmitter* GetEmitterAudioIf(bool(*)(AudioEmitter*));
        AudioEmitter* GetEmitterAudio(GameObject*);
};
