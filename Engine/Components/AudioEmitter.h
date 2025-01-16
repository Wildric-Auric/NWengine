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
    AudioEmitter(GameObject* attachedObj);

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

    /**
     * @brief SetFrequency sets sound frequency.
     * @param v the frequency value, should be between 0 et 2.
     */
    void SetFrequency(float v);

    /**
     * @brief SetLooping sets if the the sound should loop.
     * @param v The looping value.
     */
    void SetLooping(bool v);

    /**
     * @brief StopIfHasFinished Stops the sound if it has finished automatically called
     * by audio listener.
     * @param snd The sound object.
     */
    void StopIfHasFinished();

    GameObject* attachedObj = nullptr;

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

    /**
     * @brief Serialize serializes the AudioEmitter data.
     * @param data The file stream to write the serialized data to.
     * @param offset The offset in the file stream to start writing the serialized data.
     * @return The number of bytes written.
     */
    int Serialize(std::fstream* data, int offset) override;

    /**
     * @brief Deserialize deserializes the AudioEmitter data.
     * @param data The file stream to read the serialized data from.
     * @param offset The offset in the file stream to start reading the serialized data.
     * @return The number of bytes read.
     */
    int Deserialize(std::fstream* data, int offset) override;

    /**
     * @brief componentList A map of game objects and their corresponding AudioEmitter components.
     */
    static std::map<GameObject*, AudioEmitter*> componentList;
};
